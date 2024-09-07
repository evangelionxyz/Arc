#include "darray.h"

DynamicArray *da_create(size_t capacity)
{
    DynamicArray *array = (DynamicArray *)malloc(sizeof(DynamicArray));
    array->data = (void **)malloc(capacity * sizeof(void *));
    array->size = 0;
    array->capacity = capacity;
    return array;
}

void *da_push_back(DynamicArray *array, void *element)
{
    // resize the array if necessary
    if (array->size >= array->capacity)
    {
        array->capacity *= 2; // double the capacity
        size_t new_alloc_size = array->capacity * sizeof(void *);
        array->data = (void **)realloc(array->data, new_alloc_size);
    }

    // add the element
    array->data[array->size] = element;
    array->size++;

    return NULL;
}

void *da_pop_back(DynamicArray *array)
{
    if (array == NULL || array->size == 0)
        return false;

    free(array->data[array->size - 1]);
    array->data[array->size - 1] = NULL;
    array->size--;

    return true;
}

void *da_insert_element(DynamicArray *array, void *element, size_t index)
{
    if (index >= array->size) return false;
        
    if (array->size >= array->capacity)
    {
        array->capacity *= 2;
        array->data = (void **)realloc(array->data, array->capacity * sizeof(void *));
        if (!array->data)
        {
            perror("Reallocation failed");
            exit(1);
        }
    }

    // move elements to the right
    for (size_t i = array->size; i > index; --i)
    {
        array->data[i] = array->data[i - 1];
    }

    // insert the new element
    array->data[index] = element;
    array->size++;

    return true;
}

void *da_remove_element(DynamicArray *array, size_t index)
{
    if (index >= array->size) return NULL;

    void *removed_element = array->data[index];

    const size_t elements_to_move = array->size - index - 1;
    if (elements_to_move > 0)
    {
        memmove(&array->data[index], &array->data[index + 1],
            elements_to_move * sizeof(void *));
    }

    array->size--;
    array->data[array->size] = NULL;

    return removed_element;
}

void da_free(DynamicArray *array)
{
    if (!array) return false;

    free(array->data);
    free(array);
}

void *da_get_element(DynamicArray *array, size_t index)
{
    if (index >= array->size)
        return NULL;
    
    return array->data[index];
}