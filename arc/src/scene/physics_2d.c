// Copyright (c) 2024 Evangelion Manuhutu
#include "physics_2d.h"
#include "scene.h"
#include "assert.h"

static b2BodyType get_b2_body_type(BodyType2D type)
{
    switch (type)
    {
    case StaticBody2D: return b2_staticBody;
    case DynamicBody2D: return b2_dynamicBody;
    case KinematicBody2D: return b2_kinematicBody;
    }

    assert(false);
    return (b2BodyType)-1;
}

static const f32 length_units_per_meter = 1000.0f;

b2WorldId physics_2d_init()
{
    b2WorldDef world_def = { 0 };
    world_def.gravity.x              = 0.0f;
    world_def.gravity.y              = -10.0f;
    world_def.hitEventThreshold      = 1.0f * length_units_per_meter;
    world_def.restitutionThreshold   = 1.0f * length_units_per_meter;
    world_def.contactPushoutVelocity = 3.0f * length_units_per_meter;
    world_def.contactHertz           = 30.0;
    world_def.contactDampingRatio    = 10.0f;
    world_def.jointHertz             = 60.0;
    world_def.jointDampingRatio      = 2.0f;
    world_def.maximumLinearVelocity  = 400.0f * length_units_per_meter;
    world_def.enableSleep            = true;
    world_def.enableContinuous       = true;
    world_def.internalValue          = 1152023;

    return b2CreateWorld(&world_def);
}

void physics_2d_destroy(b2WorldId world_id)
{
    b2DestroyWorld(world_id);
}

void physics_2d_attach_box_collider(b2WorldId world, BoxCollider2D *box_collider, TransformComponent transform)
{
    b2BodyDef body_def        = b2DefaultBodyDef();
    body_def.type             = get_b2_body_type(box_collider->body_type);
    body_def.position.x       = transform.translation.x * PIXEL_TO_METER;
    body_def.position.y       = -transform.translation.y * PIXEL_TO_METER;
    body_def.rotation         = b2MakeRot(transform.rotation.z * DEG2RAD);
    body_def.fixedRotation    = box_collider->fixed_rotation;
    body_def.angularVelocity  = box_collider->angular_velocity;
    body_def.linearVelocity.x = box_collider->linear_velocity.x;
    body_def.linearVelocity.y = box_collider->linear_velocity.y;
    body_def.gravityScale     = box_collider->gravity_scale;
    body_def.isAwake          = true;
    body_def.isEnabled        = true;
    body_def.enableSleep      = true;
    body_def.linearDamping    = box_collider->linear_damping;
    body_def.angularDamping   = box_collider->angular_damping;
    //body_def.userData         = box_collider->user_data;
    box_collider->body_id     = b2CreateBody(world, &body_def);

    const b2Polygon box_shape = b2MakeBox((box_collider->size.x * transform.scale.x * PIXEL_TO_METER) / 2.0f,
                                          (box_collider->size.y * transform.scale.y * PIXEL_TO_METER) / 2.0f);

    b2ShapeDef shape_def  = b2DefaultShapeDef();
    shape_def.density     = box_collider->density;
    shape_def.friction    = box_collider->friction;
    shape_def.restitution = box_collider->restitution;
    shape_def.isSensor    = box_collider->is_sensor;
    shape_def.userData    = box_collider->user_data;

    box_collider->shape_id = b2CreatePolygonShape(box_collider->body_id, &shape_def, &box_shape);
}

void physics_2d_dispatch_collision_event(GameObject *go, Collision2DEvent *event)
{
    BoxCollider2D *collider = get_component(go, T_BOX_COLLIDER_2D);
    if (collider != NULL)
    {
        if (collider->event_count + 1 >= MAX_COLLISION_EVENT)
            collider->event_count = 0;
        collider->collision_events[collider->event_count] = *event;
        collider->event_count++;
    }
}

void physics_2d_simulate(Scene *scene, f32 delta_time)
{
    const i32 sub_step_count = 4;
    const f32 time_step = 1.0f / 60.0f;
    b2World_Step(scene->b2_world_id, time_step, sub_step_count);

    b2ContactEvents contact_events = b2World_GetContactEvents(scene->b2_world_id);
    for (i32 i = 0; i < contact_events.beginCount; ++i)
    {
        b2ContactBeginTouchEvent *begin_touch = contact_events.beginEvents + i;

        GameObject *go_a = b2Shape_GetUserData(begin_touch->shapeIdA);
        GameObject *go_b = b2Shape_GetUserData(begin_touch->shapeIdB);

        Collision2DEvent event_a = { .game_object = go_b, .is_begin = true };
        physics_2d_dispatch_collision_event(go_a, &event_a);

        Collision2DEvent event_b = { .game_object = go_a, .is_begin = true };
        physics_2d_dispatch_collision_event(go_b, &event_b);
    }

    for (i32 i = 0; i < contact_events.endCount; ++i)
    {
        b2ContactEndTouchEvent *end_touch = contact_events.endEvents + i;

        GameObject *go_a = b2Shape_GetUserData(end_touch->shapeIdA);
        GameObject *go_b = b2Shape_GetUserData(end_touch->shapeIdB);

        Collision2DEvent event_a = { .game_object = go_b, .is_begin = false };
        physics_2d_dispatch_collision_event(go_a, &event_a);

        Collision2DEvent event_b = { .game_object = go_a, .is_begin = false };
        physics_2d_dispatch_collision_event(go_b, &event_b);
    }

    // update physics
    for (size_t i = 0; i < scene->registry.game_objects->size; ++i)
    {
        GameObject *go = scene->registry.game_objects->data[i];
        BoxCollider2D *box_collider = get_component(go, T_BOX_COLLIDER_2D);
        if (box_collider != NULL)
        {
            TransformComponent *transform = get_transform_component(go);
            if (transform == NULL)
            {
                continue;
            }

            if (!b2Body_IsValid(box_collider->body_id))
            {
                printf("%s invalid body id\n", go->name);
                continue;
            }

            const b2Vec2 pos = b2Body_GetPosition(box_collider->body_id);
            const b2Rot rot = b2Body_GetRotation(box_collider->body_id);
            transform->translation.x = (pos.x * METER_TO_PIXEL) - transform->scale.x / 2.0f;
            transform->translation.y = -((pos.y * METER_TO_PIXEL) + transform->scale.y / 2.0f);
            transform->rotation.z = b2Rot_GetAngle(rot) * RAD2DEG;
        }
    }
}

void set_linear_velocity(BoxCollider2D *box_collider, Vector2 velocity)
{
    box_collider->linear_velocity = velocity;
    const b2Vec2 vel = { box_collider->linear_velocity.x, box_collider->linear_velocity.y };
    b2Body_SetLinearVelocity(box_collider->body_id, vel);
}

void apply_force_to_center(BoxCollider2D *box_collider, Vector2 force, bool wake)
{
    box_collider->force = force;
    const b2Vec2 f = { box_collider->force.x, box_collider->force.y };
    b2Body_ApplyForceToCenter(box_collider->body_id, f, true);
}

void apply_force(BoxCollider2D *box_collider, Vector2 force, Vector2 point, bool wake)
{
    box_collider->force = force;
    const b2Vec2 f = { box_collider->force.x, box_collider->force.y };
    const b2Vec2 p = { point.x, point.y };
    b2Body_ApplyForce(box_collider->body_id, f, p, true);
}

void set_gravity_scale(BoxCollider2D *box_collider, f32 value)
{
    box_collider->gravity_scale = value;
    b2Body_SetGravityScale(box_collider->body_id, value);
}

Vector2 get_linear_velocity(const BoxCollider2D *box_collider)
{
    b2Vec2 velocity = b2Body_GetLinearVelocity(box_collider->body_id);
    return (Vector2){ velocity.x, velocity.y };
}

f32 get_linear_damping(const BoxCollider2D *box_collider)
{
    return b2Body_GetLinearDamping(box_collider->body_id);
}

f32 get_angular_damping(const BoxCollider2D *box_collider)
{
    return b2Body_GetAngularDamping(box_collider->body_id);
}
