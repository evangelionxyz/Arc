#ifndef PLAYER_H
#define PLAYER_H

#include "core/types.h"
#include <raylib.h>
#include <raymath.h>

typedef struct GameObject GameObject;
typedef struct BoxCollider2D BoxCollider2D;
typedef struct Scene Scene;

typedef struct Player {
    GameObject *go;

    f32 move_speed;
    f32 jump_interval;
    f32 jump_cool_down;
    f32 jump_height;
    bool is_jumping;
} Player;

Player *player_create(Scene *scene);
void player_destroy(Player *player);
void player_update(Player *player, f32 delta_time);
void player_collision_2d(Player *player, BoxCollider2D *collider);

Vector3 player_get_position(Player *player);

#endif