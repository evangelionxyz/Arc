#include "components.h"

#include <stdlib.h>

void *create_component(const CompType type)
{
    switch (type)
    {
    case TypeSprite:
        {
            SpriteComponent *comp = malloc(sizeof(SpriteComponent));
            comp->tint_color = WHITE;
            comp->base.type = type;
            return comp;
        }
    case TypeTransform:
        {
            TransformComponent *comp = malloc(sizeof(TransformComponent));
            comp->base.type = type;
            return comp;
        }
    case TypeRigidbody:
        {
            Rigidbody2DComponent *comp = malloc(sizeof(Rigidbody2DComponent));
            comp->base.type = type;
            return comp;
        }
    default: return NULL;
    }
}

Texture load_sprite_texture(const char *path, const i32 width, const i32 height)
{
    Image image = LoadImage("data/textures/checkerboard.png");
    ImageResize(&image, width, height);

    const Texture texture = LoadTextureFromImage(image);

    UnloadImage(image);
    return texture;
}

void draw_sprite(const TransformComponent *transform, const SpriteComponent *sprite)
{


    if (IsTextureReady(sprite->texture))
    {
        const Rectangle src = {
            0.0f,
            0.0f,
            transform->scale.x,
            transform->scale.y,
        };

        const Rectangle dst = {
            transform->translation.x,
            transform->translation.y,
        transform->scale.x,
        transform->scale.y
        };

        DrawTexturePro(sprite->texture, src, dst,
            (Vector2){dst.width / 2.0f, dst.height / 2.0f },
            transform->angle,
            sprite->tint_color);
    }
    else
    {
        const Rectangle center = {
            transform->translation.x - transform->scale.x / 2.0f,
            transform->translation.y - transform->scale.y / 2.0f,
            transform->scale.x,
            transform->scale.y
        };

        DrawRectanglePro(center,
        (Vector2){center.width / 2.0f, center.height / 2.0f},
        transform->angle,
        sprite->tint_color);
    }
}