#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "raylib.h"
#include "core/darray.h"
#include "core/types.h"
#include "components.h"

#define MAX_COMPONENT 10
typedef struct GameObject {
    u32 component_ids[MAX_COMPONENT];
    u8 component_count;
    size_t id;
} GameObject;

typedef struct GoRegistry {
    DynamicArray *game_objects;
    DynamicArray *components;
} GoRegistry;

GameObject *create_game_object(const GoRegistry *registry, Vector2 position, Vector2 scale);
void destroy_game_object(const GoRegistry *registry, const GameObject *go);
void add_component(GameObject *go, void *component, const GoRegistry *registry);
void remove_component(GameObject *go, CompType type, const GoRegistry *registry);
void *get_component(const GameObject *go, CompType type, const GoRegistry *registry);
bool has_component(const GameObject *go, CompType type, const GoRegistry *registry);

#endif