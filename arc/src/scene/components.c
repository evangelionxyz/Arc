#include "components.h"

#include <stdlib.h>

void *create_component(const CompType type)
{
    switch (type)
    {
    case TypeSprite:
        {
            SpriteComponent *comp = malloc(sizeof(SpriteComponent));
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

void draw_sprite(const TransformComponent *transform, const SpriteComponent *sprite)
{
    const Rectangle rectangle = {
        transform->translation.x,
        transform->translation.y,
        transform->scale.x,
        transform->scale.y
    };
    const Vector2 center = { transform->scale.x / 2.0f, transform->scale.y / 2.0f };
    DrawRectanglePro(rectangle, center, transform->angle, sprite->color);
}