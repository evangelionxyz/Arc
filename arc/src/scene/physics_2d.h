// Copyright (c) 2024 Evangelion Manuhutu

#ifndef PHYSICS_2D_H
#define PHYSICS_2D_H

#include <box2d/box2d.h>
#include "components.h"

typedef struct Scene Scene;
typedef struct GameObject GameObject;

b2WorldId physics_2d_init();
void physics_2d_destroy(b2WorldId world_id);
void physics_2d_dispatch_collision_event(GameObject *go, Collision2DEvent *event);
void physics_2d_attach_box_collider(b2WorldId world, BoxCollider2D *box_collider, TransformComponent transform);
void physics_2d_simulate(Scene *scene, f32 delta_time);

void set_linear_velocity(BoxCollider2D *box_collider, Vector2 velocity);
void apply_force_to_center(BoxCollider2D *box_collider, Vector2 force, bool wake);
void apply_force(BoxCollider2D *box_collider, Vector2 force, Vector2 point, bool wake);
void set_gravity_scale(BoxCollider2D *box_collider, f32 value);
Vector2 get_linear_velocity(const BoxCollider2D *box_collider);
f32 get_linear_damping(const BoxCollider2D *box_collider);
f32 get_angular_damping(const BoxCollider2D *box_collider);


#endif //PHYSICS_2D_H
