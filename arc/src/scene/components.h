#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "raylib.h"
#include "box2d/id.h"
#include "core/types.h"

#include <stdlib.h>

typedef enum
{
    Static, Kinematic, Dynamic
} BodyType2D;

typedef enum
{
    T_NONE = 0,
    T_TRANSFORM,
    T_SPRITE,
    T_RIGIDBODY,
    T_BOX_COLLIDER
} CompType;

typedef struct
{
    CompType type;
    size_t id;
} Component;

typedef struct
{
    Component base;
    Vector3 translation;
    Vector3 scale;
    Vector3 rotation;
}TransformComponent;

typedef struct
{
    Component base;
    Color tint_color;
    Texture2D texture;
    bool textured;
} SpriteComponent;

typedef struct
{
    Component base;
} Rigidbody2DComponent;

typedef struct
{
    Component base;
    b2BodyId body_id;
    b2ShapeId shape_id;
    BodyType2D body_type;

    Vector2 offset;
    Vector2 size;
    Vector2 linear_velocity;

    float angular_velocity;
    float gravity_scale;
    float friction;
    float restitution;

    bool is_sensor;
    bool use_gravity;
    bool fixed_rotation;
} BoxCollider2DComponent;


void *create_component(CompType type);

Texture load_sprite_texture(const char *path, i32 width, i32 height);

void draw_sprite(const TransformComponent *transform, const SpriteComponent *sprite);

#endif