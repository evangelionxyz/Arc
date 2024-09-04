#ifndef SCENE_H
#define SCENE_H

#include "game_object.h"
#include "physics_2d.h"

typedef struct Scene {
    GoRegistry registry;
    Camera2D camera;
    b2WorldId b2_world_id;
} Scene;

void scene_create(Scene *scene);
void scene_update_simulation(Scene *scene, f32 delta_time);
void scene_update_render(Scene *scene, f32 delta_time);
void scene_destroy(Scene *scene);

void scene_update_camera(Camera2D *camera, Vector3 player_position, f32 delta_time);

#endif