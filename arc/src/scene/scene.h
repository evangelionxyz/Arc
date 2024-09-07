#ifndef SCENE_H
#define SCENE_H

#include "game_object.h"
#include "physics_2d.h"
#include "player.h"

typedef struct Scene
{
    GoRegistry registry;
    Camera2D camera;
    Player *player;

    b2WorldId b2_world_id;
    
    Music theme_music;
    Sound sound_effect;
} Scene;

void scene_create(Scene *scene);
void scene_update_simulation(Scene *scene, f32 delta_time);
void scene_update_render(Scene *scene, f32 delta_time);
void scene_destroy(Scene *scene);

void update_camera(Camera2D *camera, Vector3 player_position, f32 delta_time);

void create_levels(Scene *scene);

#endif