// Copyright (c) 2024 Evangelion Manuhutu

#ifndef PHYSICS_2D_H
#define PHYSICS_2D_H

#include <box2d/box2d.h>
#include "components.h"

typedef struct Scene Scene;

b2WorldId physics_2d_init();
void physics_2d_destroy(b2WorldId world_id);
void physics_2d_attach_box_collider(b2WorldId world, BoxCollider2DComponent *box_collider, TransformComponent transform);
void physics_2d_simulate(Scene *scene, float delta_time);

#endif //PHYSICS_2D_H
