#include "components.h"

#include <stdlib.h>

void *create_component(const CompType type)
{
    switch (type)
    {
    case T_SPRITE:
        {
            SpriteComponent *comp = malloc(sizeof(SpriteComponent));
            comp->tint_color      = WHITE;
            comp->base.type       = type;
            return comp;
        }
    case T_TRANSFORM:
        {
            TransformComponent *comp = malloc(sizeof(TransformComponent));
            comp->base.type   = type;
            comp->translation = (Vector3) { .x = 0.0f, .y = 0.0f, .z = 0.0f };
            comp->scale       = (Vector3) { .x = 0.0f, .y = 0.0f, .z = 0.0f };
            comp->rotation    = (Vector3) { .x = 0.0f, .y = 0.0f, .z = 0.0f };
            return comp;
        }
    case T_BOX_COLLIDER_2D:
        {
            BoxCollider2D *comp = malloc(sizeof(BoxCollider2D));
            comp->base.type        = type;
            comp->offset           = (Vector2) { 0.0f, 0.0f };
            comp->size             = (Vector2) { 1.0f, 1.0f };
            comp->restitution      = 0.6f;
            comp->friction         = 0.6f;
            comp->gravity_scale    = 1.0f;
            comp->angular_velocity = 0.0f;
            comp->density          = 1.0f;
            comp->linear_damping   = 0.1f;
            comp->angular_damping  = 0.1f;
            comp->linear_velocity  = (Vector2) { 0.0f, 0.0f };
            comp->force            = (Vector2) { 0.0f, 0.0f };
            comp->body_type        = DynamicBody2D;
            comp->is_sensor        = false;
            comp->fixed_rotation   = false;
            comp->use_gravity      = true;

            comp->user_data        = NULL;

            comp->event_count      = 0;

            return comp;
        }
    default: return NULL;
    }
}

Texture load_sprite_texture(const char *path, const i32 width, const i32 height)
{
    Image image = LoadImage(path);
    ImageResize(&image, width, height);
    const Texture texture = LoadTextureFromImage(image);
    UnloadImage(image);
    return texture;
}

void draw_sprite(const TransformComponent *transform, const SpriteComponent *sprite)
{
    const Rectangle rect = {
        transform->translation.x + transform->scale.x / 2.0f,
        transform->translation.y + transform->scale.y / 2.0f,
        transform->scale.x, transform->scale.y };
    
    if (IsTextureReady(sprite->texture))
    {
        const Rectangle destination = {
            transform->translation.x + transform->scale.x / 2.0f,
            transform->translation.y + transform->scale.y / 2.0f,
            transform->scale.x, transform->scale.y
        };

        DrawTexturePro(sprite->texture, rect, destination, (Vector2) { destination.width / 2.0f, destination.height / 2.0f }, transform->rotation.z, sprite->tint_color);
    }
    else
    {
        const Vector2 origin = { rect.width / 2.0f, rect.height / 2.0f };
        DrawRectanglePro(rect, origin, transform->rotation.z, sprite->tint_color);
    }
}


const char *get_component_type_str(CompType type)
{
    switch (type)
    {
    case T_TRANSFORM: return "Transform";
    case T_SPRITE: return "Sprite";
    case T_BOX_COLLIDER_2D: return "Box Collider 2D";
    case T_RIGIDBODY_2D: return "Rigidbody 2D";
    default: return "NONE";
    }
}
