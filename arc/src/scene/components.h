#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "raylib.h"
#include "box2d/id.h"
#include "core/types.h"

#include <stdlib.h>

typedef struct GameObject GameObject;
typedef struct Scene Scene;

typedef enum
{
    StaticBody2D, KinematicBody2D, DynamicBody2D
} BodyType2D;

typedef enum CompType
{
    T_NONE = 0,
    T_TRANSFORM,
    T_SPRITE,
    T_BOX_COLLIDER_2D,
    T_RIGIDBODY_2D
} CompType;

typedef struct Component
{
    CompType type;
    Scene *scene;
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
    bool is_hit;
} Collision2DEvent;

typedef struct BoxCollider2D
{
    Component base;
    b2BodyId body_id;
    b2ShapeId shape_id;
    BodyType2D body_type;

    Vector2 offset;
    Vector2 size;

    Vector2 linear_velocity;
    Vector2 force;
    f32 angular_velocity;
    f32 gravity_scale;
    f32 friction;
    f32 restitution;
    f32 density;
    f32 linear_damping;
    f32 angular_damping;

    bool is_sensor;
    bool use_gravity;
    bool fixed_rotation;

    i32 event_count;
    Collision2DEvent collision_events[MAX_COLLISION_EVENT];

    void *user_data;
    
} BoxCollider2D;

typedef struct Rigidbody2D
{
    Component base;
} Rigidbody2D;

void *create_component(CompType type);
Texture load_sprite_texture(const char *path, i32 width, i32 height);
void draw_sprite(const TransformComponent *transform, const SpriteComponent *sprite);

const char *get_component_type_str(CompType type);

#endif