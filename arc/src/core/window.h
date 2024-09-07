#ifndef WINDOW_H
#define WINDOW_H

#include <raylib.h>
#include "types.h"

typedef struct Window {
    i32 width;
    i32 height;
    const char *title;
    Color background_color;
} Window;

Window *window_init(i32 width, i32 height, const char *title);
Window *window_get_instance();

void window_shutdown(Window *window);
void window_update(Window *window);
void window_set_title(Window *window, const char *new_title);
i32 window_is_looping(Window *window);

#endif