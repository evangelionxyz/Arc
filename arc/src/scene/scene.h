#ifndef SCENE_H
#define SCENE_H

#include "game_object.h"

typedef struct Scene {
    GoRegistry registry;
    Camera2D camera;
} Scene;

void scene_create(Scene *scene);
void scene_update_simulation(Scene *scene, f32 delta_time);
void scene_update_render(Scene *scene, f32 delta_time);
void scene_destroy(const Scene *scene);

#endif