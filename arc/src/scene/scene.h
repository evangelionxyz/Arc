#ifndef SCENE_H
#define SCENE_H

#include "game_object.h"

typedef struct Scene {
    GoRegistry registry;
} Scene;

void scene_create(Scene *scene);
void scene_update_simulation(const Scene *scene, float delta_time);
void scene_update_render(const Scene *scene, float delta_time);
void scene_destroy(const Scene *scene);


#endif