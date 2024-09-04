// Copyright (c) 2024 Evangelion Manuhutu
#include "physics_2d.h"
#include "scene.h"
#include "assert.h"

static b2BodyType get_b2_body_type(BodyType2D type)
{
    switch (type)
    {
    case Static: return b2_staticBody;
    case Dynamic: return b2_dynamicBody;
    case Kinematic: return b2_kinematicBody;
    }

    assert(false);
    return (b2BodyType)-1;
}

static const f32 length_units_per_meter = 1000.0f;

b2WorldId physics_2d_init()
{
    b2WorldDef world_def = { 0 };
    world_def.gravity.x = 0.0f;
    world_def.gravity.y = -10.0f;
    world_def.hitEventThreshold = 1.0f * length_units_per_meter;
    world_def.restitutionThreshold = 1.0f * length_units_per_meter;
    world_def.contactPushoutVelocity = 3.0f * length_units_per_meter;
    world_def.contactHertz = 30.0;
    world_def.contactDampingRatio = 10.0f;
    world_def.jointHertz = 60.0;
    world_def.jointDampingRatio = 2.0f;
    // 400 meters per second, faster than the speed of sound
    world_def.maximumLinearVelocity = 400.0f * length_units_per_meter;
    world_def.enableSleep = true;
    world_def.enableContinuous = true;
    world_def.internalValue = 1152023;

    return b2CreateWorld(&world_def);
}

void physics_2d_destroy(b2WorldId world_id)
{
    b2DestroyWorld(world_id);
}

void physics_2d_attach_box_collider(b2WorldId world, BoxCollider2DComponent *box_collider, TransformComponent transform)
{
    b2BodyDef body_def        = b2DefaultBodyDef();
    body_def.type             = get_b2_body_type(box_collider->body_type);

    body_def.position.x       = transform.translation.x;
    body_def.position.y       = -transform.translation.y;
    body_def.rotation         = b2MakeRot(transform.rotation.z);
    body_def.gravityScale     = box_collider->gravity_scale;
    body_def.fixedRotation    = box_collider->fixed_rotation;
    body_def.angularVelocity  = box_collider->angular_velocity;
    body_def.linearVelocity.x = box_collider->linear_velocity.x;
    body_def.linearVelocity.y = box_collider->linear_velocity.y;
    body_def.gravityScale     = box_collider->gravity_scale;
    body_def.isAwake          = true;
    body_def.isEnabled        = true;
    body_def.enableSleep      = true;
    box_collider->body_id     = b2CreateBody(world, &body_def);

    // Adjust the size to consider scaling properly
    const b2Polygon box_shape = b2MakeBox((box_collider->size.x * transform.scale.x) / 2.0f,
                                          (box_collider->size.y * transform.scale.y) / 2.0f);

    b2ShapeDef shape_def  = b2DefaultShapeDef();
    shape_def.friction    = box_collider->friction;
    shape_def.restitution = box_collider->restitution;
    shape_def.isSensor    = box_collider->is_sensor;

    b2CreatePolygonShape(box_collider->body_id, &shape_def, &box_shape);
}

void physics_2d_simulate(Scene *scene, float delta_time)
{
    const int sub_step_count = 4;
    b2World_Step(scene->b2_world_id, 1.0f / 60.0f, sub_step_count);

    // update physics
    for (size_t i = 0; i < scene->registry.game_objects->size; ++i)
    {
        GameObject *go = scene->registry.game_objects->data[i];
        BoxCollider2DComponent *box_collider = get_component(go, T_BOX_COLLIDER, scene);
        if (box_collider != NULL)
        {
            
            TransformComponent *transform = get_transform_component(go, scene);
            if (transform == NULL)
                continue;

            const b2Vec2 vel = { box_collider->linear_velocity.x, -box_collider->linear_velocity.y };
            b2Body_SetLinearVelocity(box_collider->body_id, vel);

            const b2Vec2 pos         = b2Body_GetPosition(box_collider->body_id);
            const b2Rot rot          = b2Body_GetRotation(box_collider->body_id);
            transform->translation.x = pos.x - transform->scale.x / 2.0f;
            transform->translation.y = -(pos.y + transform->scale.y / 2.0f);
            transform->rotation.z    = b2Rot_GetAngle(rot) * 57.2957795;
        }
    }
}

