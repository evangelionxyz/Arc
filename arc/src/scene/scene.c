#include "scene.h"

#include <math.h>
#include <stdio.h>

#include "core/window.h"

void scene_create(Scene *scene)
{
    const Window *window = window_get_instance();


    // TODO: load scene from file (YAML)

    // register 10 objects for initialization
    scene->registry.game_objects = da_create(10);
    scene->registry.components = da_create(10);

    const Vector2 scale = { 50, 50 };
    const Vector2 pos = { 0.0f, 0.0 };
    const i32 count = 5;

    for (i32 index = -count; index < count; index++)
    {
        const Vector2 offset_pos = {
            .x = window->width / 2.0f + index * scale.x * 1.5f,
            .y = window->height / 2.0f + index * scale.x * 1.5f
        };

        GameObject *go = create_game_object("quad", &scene->registry, offset_pos, scale);
        SpriteComponent *sprite = create_component(TypeSprite);
        sprite->tint_color = (index + count) % 2 == 0 ? GREEN : BLUE;
        add_component(go, sprite, &scene->registry);
    }

    GameObject *go = create_game_object("player", &scene->registry, pos, scale);
    SpriteComponent *sprite = create_component(TypeSprite);
    sprite->texture = load_sprite_texture("data/textures/checkerboard.png", (i32)scale.x, (i32)scale.y);
    add_component(go, sprite, &scene->registry);

    const Vector2 cam_offset = { window->width / 2.0f, window->height / 2.0f };
    scene->camera.offset = cam_offset;
    scene->camera.target = (Vector2){0.0f, 0.0f};
    scene->camera.rotation = 0.0f;
    scene->camera.zoom = 1.0f;
}

void scene_update_simulation(Scene *scene, const f32 delta_time)
{
    const f32 speed = 200.0f;

    for (size_t i = 0; i < scene->registry.game_objects->size; ++i)
    {
        const void *go = scene->registry.game_objects->data[i];
        if (go != NULL)
        {
            void *comp = get_component(go, TypeTransform, &scene->registry);
            if (comp != NULL)
            {
                TransformComponent *transform = comp;
                // transform->angle += delta_time * speed;
            }
        }
    }

    Vector3 velocity = {0.0f, 0.0f };
    if (IsKeyDown(KEY_W)) velocity.y -= 1.0f;
    else if (IsKeyDown(KEY_S)) velocity.y += 1.0f;
    if (IsKeyDown(KEY_A)) velocity.x -= 1.0f;
    else if (IsKeyDown(KEY_D)) velocity.x += 1.0f;

    static Vector2 player_position = {0.0f, 0.0f };

    GameObject *player = get_game_object_by_name("player", &scene->registry);
    if (player != NULL)
    {
        void *comp = get_component(player, TypeTransform, &scene->registry);
        if (comp != NULL)
        {
            TransformComponent *transform = comp;
            transform->translation.x += velocity.x * delta_time * speed;
            transform->translation.y += velocity.y * delta_time * speed;
            player_position.x = transform->translation.x;
            player_position.y = transform->translation.y;
        }
    }

    const float offset = 70.0f;
    if (player_position.x < scene->camera.target.x - offset)
        scene->camera.target.x -= 2.5f;
    else if (player_position.x > scene->camera.target.x + offset)
        scene->camera.target.x += 2.5f;
    if (player_position.y < scene->camera.target.y - offset)
        scene->camera.target.y -= 2.5f;
    else if (player_position.y > scene->camera.target.y + offset)
        scene->camera.target.y += 2.5f;
}

void scene_update_render(Scene *scene, const f32 delta_time)
{
    BeginMode2D(scene->camera);

    for (size_t i = 0; i < scene->registry.game_objects->size; ++i)
    {
        const void *go = scene->registry.game_objects->data[i];
        if (go != NULL)
        {
            const void *transform = get_component(go, TypeTransform, &scene->registry);
            if (transform != NULL)
            {
                // sprite component
                const void *sprite = get_component(go, TypeSprite, &scene->registry);
                if (sprite != NULL) draw_sprite(transform, sprite);
            }
        }
    }

    EndMode2D();
}

void scene_destroy(const Scene *scene)
{
    for (size_t i = 0; i < scene->registry.game_objects->size; ++i)
    {
        GameObject *go = scene->registry.game_objects->data[i];
        destroy_game_object(&scene->registry, go);
    }

    da_free(scene->registry.game_objects);
    da_free(scene->registry.components);
}
