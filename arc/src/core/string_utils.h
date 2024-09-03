#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

char *str_concat(char *first, char *second)
{
    int size = strlen(first) + strlen(second) + 1; // include null terminator
    char *result = (char *)malloc(sizeof(char) * size );
    strcpy(result, first);
    strcat(result, second);

    return result;
}

char *str_from_format(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    int size = vsnprintf(NULL, 0, format, args) + 1; // include the null terminator
    if (size <= 0)
        return NULL;

    char *result = (char *)malloc(size);
    if (result == NULL)
        return NULL;

    va_start(args, format);
    vsnprintf(result, size, format, args);
    va_end(args);

    return result;
}

#endif