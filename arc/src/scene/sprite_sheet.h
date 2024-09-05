#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include "core/types.h"
#include "core/darray.h"
#include "raylib.h"

typedef struct
{
    const char *filepath;
    DynamicArray positions;
    Texture2D texture;
} SpriteSheet;


#endif