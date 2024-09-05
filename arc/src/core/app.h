#ifndef APP_H
#define APP_H

#include "window.h"
#include "scene/scene.h"

typedef struct Application {
    Window *window;
    Scene scene;
    f32 time_seconds;
} Application;

Application *app_create(i32 argc, char **argv);
Application *app_get_instance();

void app_run(Application *app);
void app_close(Application *app);
void app_update_simulation(Application *app, f32 delta_time);
void app_update_render(Application *app, f32 delta_time);
void app_render_gui(Application *app, f32 delta_time);

#endif