#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "raylib.h"
#include "box2d/id.h"
#include "core/types.h"

#include <stdlib.h>

typedef struct GameObject GameObject;

typedef enum
{
    Static, Kinematic, Dynamic
} BodyType2D;

typedef enum CompType
{
    T_NONE = 0,
    T_TRANSFORM,
    T_SPRITE,
    T_BOX_COLLIDER
} CompType;

typedef struct Component
{
    CompType type;
    size_t id;
} Component;

typedef struct TransformComponent
{
    Component base;
    Vector3 translation;
    Vector3 scale;
    Vector3 rotation;
}TransformComponent;

typedef struct SpriteComponent
{
    Component base;
    Color tint_color;
    Texture2D texture;
    bool textured;
} SpriteComponent;

#define MAX_COLLISION_EVENT 100
typedef struct Collision2DEvent {
    GameObject *game_object;
    bool is_begin;
} Collision2DEvent;

typedef struct BoxCollider2DComponent
{
    Component base;
    b2BodyId body_id;
    b2ShapeId shape_id;
    BodyType2D body_type;

    Vector2 offset;
    Vector2 size;
    Vector2 linear_velocity;

    f32 angular_velocity;
    f32 gravity_scale;
    f32 friction;
    f32 restitution;

    bool is_sensor;
    bool use_gravity;
    bool fixed_rotation;

    i32 event_count;
    Collision2DEvent collision_events[MAX_COLLISION_EVENT];

    void *user_data;
    
} BoxCollider2DComponent;


void *create_component(CompType type);
Texture load_sprite_texture(const char *path, i32 width, i32 height);
void draw_sprite(const TransformComponent *transform, const SpriteComponent *sprite);

#endif