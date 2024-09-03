#include "window.h"
#include <stdlib.h>

static Window *s_Window = NULL;

Window *window_init(i32 width, i32 height, const char *title)
{
    s_Window = malloc(sizeof(Window));
    s_Window->width = width;
    s_Window->height = height;
    s_Window->title = title;

    InitWindow(width, height, title);
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    return s_Window;
}

Window *window_get_instance()
{
    return s_Window;
}

void window_shutdown(Window *window)
{
    CloseWindow();

    if (window)
    {
        free(window);
        window = NULL;
    }
}

void window_set_title(Window *window, const char *new_title)
{
    window->title = new_title;
    SetWindowTitle(new_title);
}

i32 window_is_looping(Window *window)
{
    return WindowShouldClose() == 0;
}

void window_clear(Window *window)
{
    ClearBackground(window->background_color);
}

void window_poll_events(Window *window)
{
    PollInputEvents();
}