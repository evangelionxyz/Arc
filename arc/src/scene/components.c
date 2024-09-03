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

void destroy_component(void *component)
{
    if (component == NULL)
        return;

    Component *comp = component;
    switch (comp->type)
    {
    case TypeSprite:
        {
            const SpriteComponent *sc = component;
            UnloadTexture(sc->texture);
            free(comp);
            break;
        }
    case TypeTransform:
    case TypeRigidbody:
        {
            free(comp);
            break;
        }
    default: break;
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
    const Rectangle rectangle = {
        transform->translation.x - transform->scale.x / 2.0f,
        transform->translation.y - transform->scale.y / 2.0f,
        transform->scale.x,
        transform->scale.y
    };

    if (IsTextureReady(sprite->texture))
        DrawTexture(sprite->texture, rectangle.x, rectangle.y, sprite->tint_color);
    else DrawRectanglePro(rectangle,
        (Vector2){rectangle.width / 2.0f, rectangle.height / 2.0f},
        transform->angle,
        sprite->tint_color);
}