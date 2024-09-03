#include "scene.h"
#include "core/window.h"

void scene_create(Scene *scene)
{
    const Window *window = window_get_instance();

    // TODO: load scene from file (YAML)

    // register 10 objects for initialization
    scene->registry.game_objects = da_create(10);
    scene->registry.components = da_create(10);

    const Vector2 scale = {
        .x = 50,
        .y = 50
    };

    const i32 count = 5;

    for (i32 index = -count; index < count; index++)
    {
        const Vector2 pos = {
            .x = window->width / 2.0f + index * scale.x * 1.5f,
            .y = window->height / 2.0f + index * scale.x * 1.5f
        };

        GameObject *go = create_game_object(&scene->registry, pos, scale);
        SpriteComponent *sprite = create_component(TypeSprite);
        add_component(go, sprite, &scene->registry);
    }

    const Vector2 pos = {
        .x = window->width / 2.0f,
        .y = window->height / 2.0f
    };

    GameObject *go = create_game_object(&scene->registry, pos, scale);
    SpriteComponent *sprite = create_component(TypeSprite);
    sprite->texture = load_sprite_texture("data/textures/checkerboard.png", (i32)scale.x, (i32)scale.y);
    add_component(go, sprite, &scene->registry);
}

void scene_update_simulation(const Scene *scene, const f32 delta_time)
{
    Vector3 velocity = {0.0f, 0.0f };
    if (IsKeyDown(KEY_W))
        velocity.y -= 1.0f;
    else if (IsKeyDown(KEY_S))
        velocity.y += 1.0f;
    if (IsKeyDown(KEY_A))
        velocity.x -= 1.0f;
    else if (IsKeyDown(KEY_D))
        velocity.x += 1.0f;

    for (size_t i = 0; i < scene->registry.game_objects->size; ++i)
    {
        const void *go = scene->registry.game_objects->data[i];
        if (go != NULL)
        {
            void *comp = get_component(go, TypeTransform, &scene->registry);
            if (comp != NULL)
            {
                const f32 speed = 100.0f;
                TransformComponent *transform = comp;
                transform->translation.x += velocity.x * delta_time * speed;
                transform->translation.y += velocity.y * delta_time * speed;
                transform->angle += delta_time * speed;
            }
        }
    }
}

void scene_update_render(const Scene *scene, const f32 delta_time)
{
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
}

void scene_destroy(const Scene *scene)
{
    for (size_t i = 0; i < scene->registry.game_objects->size; ++i)
    {
        GameObject *go = scene->registry.game_objects->data[i];
        for (u8 j = 0; j < go->component_count; ++j)
        {
            void *comp = get_component(go, TypeTransform, &scene->registry);
            if (comp != NULL)
                destroy_component(comp);
        }

        destroy_game_object(&scene->registry, go);
    }
    da_free(scene->registry.game_objects);
    da_free(scene->registry.components);
}
