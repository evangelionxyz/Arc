#include "scene.h"

#include <math.h>
#include <stdio.h>
#include <raymath.h>

#include "core/window.h"

void scene_create(Scene *scene)
{
    i32 screen_width = GetScreenWidth();
    i32 screen_height = GetScreenHeight();

    // load resources
    scene->theme_music = LoadMusicStream("data/sounds/theme.mp3");
    scene->sound_effect = LoadSound("data/sounds/vine-boom.mp3");
    PlayMusicStream(scene->theme_music);

    // create physics 2d world
    scene->b2_world_id = physics_2d_init();

    // register 10 objects for initialization
    scene->registry.game_objects = da_create(10);
    scene->registry.components = da_create(10);

    Vector3 pos = { 0.0f, -150.0f, 0.0f };
    Vector3 rotation = Vector3Zero();
    Vector3 scale = { 30, 30.0f, 1.0f };

    {
        GameObject *go = create_game_object("player", scene, pos, scale, rotation);
        SpriteComponent *sprite = create_component(T_SPRITE);
        sprite->texture = load_sprite_texture("data/textures/checkerboard.png", (i32)scale.x, (i32)scale.y);
        sprite->tint_color = BLUE;
        add_component(go, sprite, scene);

        BoxCollider2DComponent *collider = create_component(T_BOX_COLLIDER);
        collider->gravity_scale = 1060.0f; // m/ s*2
        collider->restitution = 10.0f;
        collider->friction = 0.0f;
        collider->body_type = Dynamic;
        collider->user_data = go;
        physics_2d_attach_box_collider(scene->b2_world_id, collider, *get_transform_component(go, scene));
        add_component(go, collider, scene);
    }

    for (i32 i = 0; i < 10; i++)
    {
        pos.y = -100.0f;
        pos.x = i * scale.x;
        GameObject *go = create_game_object(i % 2 == 0 ? "enemy" : "no_enemy", scene, pos, scale, rotation);
        SpriteComponent *sprite = create_component(T_SPRITE);
        sprite->tint_color = BLUE;
        add_component(go, sprite, scene);

        BoxCollider2DComponent *collider = create_component(T_BOX_COLLIDER);
        collider->body_type = Dynamic;
        collider->restitution = 5.0f;
        collider->friction = 0.0f;
        collider->gravity_scale = 1000.0f;
        collider->user_data = go;
        physics_2d_attach_box_collider(scene->b2_world_id, collider, *get_transform_component(go, scene));
        add_component(go, collider, scene);
    }

    {
        scale.x = 1000.0f;
        scale.y = 50.0f;

        pos.x = 0.0f;
        pos.y = 100.0f;

        GameObject *go = create_game_object("floor", scene, pos, scale, rotation);
        BoxCollider2DComponent *collider = create_component(T_BOX_COLLIDER);
        collider->body_type = Static;
        collider->restitution = 5.0f;
        collider->friction = 0.0f;
        collider->user_data = go;
        physics_2d_attach_box_collider(scene->b2_world_id, collider, *get_transform_component(go, scene));
        add_component(go, collider, scene);
        SpriteComponent *sprite= create_component(T_SPRITE);
        //sprite->texture = load_sprite_texture("data/textures/checkerboard.png", (i32)scale.x, (i32)scale.y);
        sprite->tint_color = MAROON;
        add_component(go, sprite, scene);
    }
    

    const Vector2 cam_offset = { screen_width / 2.0f, screen_height / 2.0f };
    scene->camera.offset = cam_offset;
    scene->camera.target = (Vector2){0.0f, 0.0f};
    scene->camera.rotation = 0.0f;
    scene->camera.zoom = 1.0f;
}

void scene_update_simulation(Scene *scene, const f32 delta_time)
{
    // update audio
    UpdateMusicStream(scene->theme_music);

    const f32 speed = 200.0f;

    Vector2 velocity = {0.0f, 0.0f };
    if (IsKeyDown(KEY_W)) velocity.y -= 1.0f;
    else if (IsKeyDown(KEY_S)) velocity.y += 1.0f;
    if (IsKeyDown(KEY_A)) velocity.x -= 1.0f;
    else if (IsKeyDown(KEY_D)) velocity.x += 1.0f;

    GameObject *player = get_game_object_by_name("player", scene);
    if (player != NULL)
    {
        TransformComponent *transform = get_transform_component(player, scene);
        BoxCollider2DComponent *collider = get_component(player, T_BOX_COLLIDER, scene);
        if (transform != NULL && collider != NULL)
        {
            for (int i = 0; i < collider->event_count; ++i)
            {
                Collision2DEvent* event = &collider->collision_events[i];
                if (event->is_begin)
                {
                    PlaySound(scene->sound_effect);

                    if (strcmp(event->game_object->name, "enemy") == 0)
                        destroy_game_object(event->game_object, scene);
                }
                    
            }
            // Clear events after processing
            collider->event_count = 0;
            
            collider->linear_velocity = Vector2Add(collider->linear_velocity, velocity);
            scene_update_camera(&scene->camera, transform->translation, delta_time);
        }
    }

    // update physics
    physics_2d_simulate(scene, delta_time);
}

void scene_update_render(Scene *scene, const f32 delta_time)
{
    BeginMode2D(scene->camera);

    for (size_t i = 0; i < scene->registry.game_objects->size; ++i)
    {
        const void *go = scene->registry.game_objects->data[i];
        if (go != NULL)
        {
            const void *transform = get_component(go, T_TRANSFORM, scene);
            if (transform != NULL)
            {
                // sprite component
                const void *sprite = get_component(go, T_SPRITE, scene);
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

    physics_2d_destroy(scene->b2_world_id);
}

void scene_update_camera(Camera2D *camera, Vector3 player_position, f32 delta_time)
{
    const float offset = 70.0f;
    //if (player_position.x < camera->target.x - offset)
    //    camera->target.x -= 2.5f;
    //else if (player_position.x > camera->target.x + offset)
    //    camera->target.x += 2.5f;
    //if (player_position.y < camera->target.y - offset)
    //    camera->target.y -= 2.5f;
    //else if (player_position.y > camera->target.y + offset)
    //    camera->target.y += 2.5f;

    camera->target.x = player_position.x;
    camera->target.y = player_position.y;
}
