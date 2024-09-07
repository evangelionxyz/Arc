#include "scene.h"
#include "core/window.h"
#include "components.h"

#include <math.h>
#include <stdio.h>
#include <raymath.h>

void scene_create(Scene *scene)
{
    // load resources
    scene->theme_music = LoadMusicStream("data/sounds/theme.mp3");
    scene->sound_effect = LoadSound("data/sounds/vine-boom.mp3");
    PlayMusicStream(scene->theme_music);

    const Vector2 cam_offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    scene->camera.offset = cam_offset;
    scene->camera.target = (Vector2){0.0f, 0.0f};
    scene->camera.rotation = 0.0f;
    scene->camera.zoom = 1.0f;

    // create physics 2d world
    scene->b2_world_id = physics_2d_init();

    // register 10 objects for initialization
    scene->registry.game_objects = da_create(100);
    scene->registry.components = da_create(100);

    // create player
    scene->player = player_create(scene);

    create_levels(scene);

#if 0
    for (size_t i = 0; i < scene->registry.game_objects->size; i++)
    {
        GameObject *go = scene->registry.game_objects->data[i];
        printf("%zu: %s components:\n", go->id, go->name);

        for (size_t j = 0; j < go->component_count; j++)
        {
            size_t id = go->component_ids[j];
            Component *comp = scene->registry.components->data[id];
            printf("   %zu: %s\n", comp->id, get_component_type_str(comp->type));
        }
        printf("\n");
    }
#endif
}

void scene_update_simulation(Scene *scene, const f32 delta_time)
{
    // update audio
    UpdateMusicStream(scene->theme_music);

    // update physics
    physics_2d_simulate(scene, delta_time);

    player_update(scene->player, delta_time);
    update_camera(&scene->camera, player_get_position(scene->player), delta_time);
}

void scene_update_render(Scene *scene, const f32 delta_time)
{
    BeginMode2D(scene->camera);
    for (size_t i = 0; i < scene->registry.game_objects->size; ++i)
    {
        GameObject *go = scene->registry.game_objects->data[i];
        if (go != NULL)
        {
            TransformComponent *transform = get_transform_component(go);
            if (transform != NULL)
            {
                // sprite component
                SpriteComponent *sprite = get_component(go, T_SPRITE);
                if (sprite != NULL) draw_sprite(transform, sprite);
            }
        }
    }
    EndMode2D();
}

void scene_destroy(Scene *scene)
{
    UnloadMusicStream(scene->theme_music);
    UnloadSound(scene->sound_effect);

    for (size_t i = 0; i < scene->registry.game_objects->size; i++)
    {
        GameObject *go = scene->registry.game_objects->data[i];
        destroy_game_object(go, scene);
    }

    da_free(scene->registry.game_objects);
    da_free(scene->registry.components);

    // destroy player
    player_destroy(scene->player);

    physics_2d_destroy(scene->b2_world_id);
}

void update_camera(Camera2D *camera, Vector3 player_pos, f32 delta_time)
{
    const float offset = 50.0f;
    const float min_x = camera->target.x - offset;
    const float max_x = camera->target.x + offset;
    const float min_y = camera->target.y - offset / 2.0f;
    const float max_y = camera->target.y + offset / 2.0f;

    const float speed = Vector2Distance((Vector2){player_pos.x, player_pos.y}, 
                        camera->target) //(Vector2){max_x + min_x, max_y - min_y})
                        * PIXEL_TO_METER * 6.0f;

    if (player_pos.x < min_x)
    {
        camera->target.x -= (float)fabs(speed);
    }
        
    else if (player_pos.x > max_x)
    {
        camera->target.x += (float)fabs(speed);
    }
        
    if (player_pos.y < min_y)
    {
        camera->target.y -= (float)fabs(speed);
    }
        
    else if (player_pos.y > max_y)
    {
        camera->target.y += (float)fabs(speed);
    }
        
}

void create_levels(Scene *scene)
{
    Vector3 pos = { 0.0f, -150.0f, 0.0f };
    Vector3 rotation = Vector3Zero();
    Vector3 scale = { 30, 30.0f, 1.0f };

    i32 count = 100;
    for (i32 i = -count; i < count; i++)
    {
        pos.y = -100.0f;
        pos.x = scale.x + i * scale.x + (scale.x / 2.0f);
        GameObject *go = create_game_object("enemy", scene, pos, scale, rotation);
        SpriteComponent *sprite = create_component(T_SPRITE);
        sprite->tint_color = i % 2 == 0 ? RED : GREEN;
        add_component(go, sprite);

        BoxCollider2D *collider = create_component(T_BOX_COLLIDER_2D);
        collider->body_type = DynamicBody2D;
        collider->gravity_scale = 0.0f;
        collider->user_data = go;
        physics_2d_attach_box_collider(scene->b2_world_id, collider, *get_transform_component(go));
        add_component(go, collider);
    }

    // platform
    {
        scale.x = 10000.0f;
        scale.y = 100.0f;
        pos.x = 0.0f;
        pos.y = 150.0f;
        GameObject *go = create_game_object("platform", scene, pos, scale, rotation);

        BoxCollider2D *collider = create_component(T_BOX_COLLIDER_2D);
        collider->body_type = StaticBody2D;
        collider->gravity_scale = 0.0f;
        collider->user_data = go;
        physics_2d_attach_box_collider(scene->b2_world_id, collider, *get_transform_component(go));
        add_component(go, collider);

        SpriteComponent *sprite = create_component(T_SPRITE);
        sprite->tint_color = DARKBROWN;
        add_component(go, sprite);
    }

    // obstacle
    for (i32 i = 0; i < 20; ++i)
    {
        scale.x = 100.0f;
        scale.y = 50.0f;
        pos.x = (i * scale.x + scale.x);
        pos.y = 100.0f - (i * scale.y + scale.y);
        GameObject *go = create_game_object("platform", scene, pos, scale, rotation);

        BoxCollider2D *collider = create_component(T_BOX_COLLIDER_2D);
        collider->body_type = StaticBody2D;
        collider->gravity_scale = 0.0f;
        collider->user_data = go;
        physics_2d_attach_box_collider(scene->b2_world_id, collider, *get_transform_component(go));
        add_component(go, collider);

        SpriteComponent *sprite = create_component(T_SPRITE);
        sprite->tint_color = RED;
        add_component(go, sprite);
    }
}
