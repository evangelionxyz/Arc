#include "app.h"
#include "string_utils.h"

static Application *s_App = NULL;

Application *app_create(i32 argc, char **argv)
{
    s_App = malloc(sizeof(Application));
    return s_App;
}

Application *app_get_instance()
{
    return s_App;
}

void app_run(Application *app)
{
    app->window = window_init(1080, 540, "Arc");
    InitAudioDevice();

    app->window->background_color = RAYWHITE;
    app->time_seconds = 0.0f;
    scene_create(&app->scene);

    f32 last_frame_time = 0.0f;
    while(window_is_looping(app->window)) {
        app->time_seconds = GetTime();
        f32 delta_time = app->time_seconds - last_frame_time;
        last_frame_time = app->time_seconds;

        window_poll_events(app->window);
        app_update_simulation(app, delta_time);

        BeginDrawing();
        {
            window_clear(app->window);
            app_render_gui(app, delta_time);

            app_update_render(app, delta_time);
        }
        EndDrawing();
    }

    CloseAudioDevice();
}

void app_update_simulation(Application *app, f32 delta_time)
{
    scene_update_simulation(&app->scene, delta_time);
}

void app_update_render(Application *app, f32 delta_time)
{
    scene_update_render(&app->scene, delta_time);
}

void app_close(Application *app)
{
    if (app)
    {
        scene_destroy(&app->scene);
        window_shutdown(app->window);
        free(app);
    }
        
}

void app_render_gui(Application *app, f32 delta_time)
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
