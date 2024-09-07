#include "game.h"
#include "string_utils.h"

static Game *s_Game = NULL;

Game *game_create(i32 argc, char **argv)
{
    s_Game = malloc(sizeof(Game));
    return s_Game;
}

Game *game_get_instance()
{
    return s_Game;
}

void game_run(Game *game)
{
    game->window = window_init(1080, 540, "Arc");
    InitAudioDevice();

    game->window->background_color = RAYWHITE;
    game->time_seconds = 0.0f;
    scene_create(&game->scene);

    f32 last_frame_time = 0.0f;
    while(window_is_looping(game->window)) {
        game->time_seconds = GetTime();
        f32 delta_time = game->time_seconds - last_frame_time;
        last_frame_time = game->time_seconds;

        game_update_simulation(game, delta_time);

        BeginDrawing();
        {
            window_update(game->window);
            game_update_render(game, delta_time);

            game_render_gui(game, delta_time);
        }
        EndDrawing();
    }

    CloseAudioDevice();
}

void game_update_simulation(Game *game, f32 delta_time)
{
    scene_update_simulation(&game->scene, delta_time);
}

void game_update_render(Game *game, f32 delta_time)
{
    scene_update_render(&game->scene, delta_time);
}

void game_close(Game *game)
{
    if (game)
    {
        scene_destroy(&game->scene);
        window_shutdown(game->window);
        free(game);
    }
        
}

void game_render_gui(Game *game, f32 delta_time)
{
    static f32 refresh_time = 0.0f;
    refresh_time += delta_time;

    static const char *fps_counter = "";
    if (refresh_time >= 0.25f)
    {
        refresh_time = 0.0f;
        fps_counter = str_from_format("FPS: %.2f", 1.0f / delta_time);
    }

    DrawText(fps_counter, 20, 20, 32, RED);
    
}
