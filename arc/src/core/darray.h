#ifndef DARRAY_H
#define DARRAY_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct DynamicArray
{
    void **data;
    size_t size;
    size_t capacity;
} DynamicArray;


DynamicArray *da_create(size_t capacity);
void *da_push_back(DynamicArray *array, void *element);
void *da_pop_back(DynamicArray *array);
void *da_insert_element(DynamicArray *array, void *element, size_t index);
void *da_remove_element(DynamicArray *array, size_t index);
void da_free(DynamicArray *array);
void *da_get_element(DynamicArray *array, size_t index);

#endif