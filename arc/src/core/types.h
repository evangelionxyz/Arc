#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stdint.h>

#define PIXEL_TO_METER 0.01f  // 1 pixel = 0.02 meters (or 50 pixels = 1 meter)
#define METER_TO_PIXEL 100.0f  // Reverse of the above

typedef unsigned char byte;
typedef unsigned short ushort;

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef long long i64;
typedef int i32;
typedef short i16;
typedef char i8;

typedef float f32;
typedef double f64;

#endif