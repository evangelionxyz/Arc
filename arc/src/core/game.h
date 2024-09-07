#ifndef APP_H
#define APP_H

#include "window.h"
#include "scene/scene.h"

typedef struct Game {
    Window *window;
    Scene scene;
    f32 time_seconds;
} Game;

Game *game_create(i32 argc, char **argv);
Game *game_get_instance();

void game_run(Game *game);
void game_close(Game *game);
void game_update_simulation(Game *game, f32 delta_time);
void game_update_render(Game *game, f32 delta_time);
void game_render_gui(Game *game, f32 delta_time);

#endif