#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "raylib.h"
#include "core/types.h"

typedef enum CompType
{
    TypeNone = 0,
    TypeTransform,
    TypeSprite,
    TypeRigidbody
} CompType;

typedef struct Component
{
    CompType type;
    size_t id;
} Component;

typedef struct TransformComponent
{
    Component base;
    Vector2 translation;
    Vector2 scale;
    f32 angle;
}TransformComponent;

typedef struct SpriteComponent
{
    Component base;
    Color tint_color;
    Texture2D texture;
} SpriteComponent;

typedef struct Rigidbody2DComponent
{
    Component base;
} Rigidbody2DComponent;

void *create_component(CompType type);
void destroy_component(void *component);

Texture load_sprite_texture(const char *path, i32 width, i32 height);

void draw_sprite(const TransformComponent *transform, const SpriteComponent *sprite);

#endif