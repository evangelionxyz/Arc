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
    app->window = window_init(600, 400, "Arc");
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
            app_update_render(app, delta_time);
        }
        EndDrawing();
    }
}

void app_update_simulation(Application *app, f32 delta_time)
{
    static f32 refresh_time = 0.0f;
    refresh_time += delta_time;
    if (refresh_time >= 0.25f)
    {
        const char *new_title = str_from_format("Arc - %.2f FPS", 1.0f / delta_time);
        window_set_title(app->window, new_title);
        refresh_time = 0.0f;
    }

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
