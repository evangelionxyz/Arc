#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "raylib.h"
#include "core/darray.h"
#include "core/types.h"
#include "components.h"

#define MAX_COMPONENT 10
static const INVALID_COMPONENT_ID = (size_t)-1;

typedef struct Scene Scene;

typedef struct GameObject {
    size_t component_ids[MAX_COMPONENT];
    u8 component_count;
    size_t id;
    const char *name;
    Scene *scene;
} GameObject;

typedef struct {
    DynamicArray *game_objects;
    DynamicArray *components;
} GoRegistry;

GameObject *create_game_object(const char *name, Scene *scene, const Vector3 position, const Vector3 scale, const Vector3 rotation);
GameObject *get_game_object_by_name(const char *name, const Scene *scene);
void destroy_game_object(GameObject *go, Scene *scene);

void *add_component(GameObject *go, void *component);
void *remove_component(GameObject *go, CompType type);

TransformComponent *get_transform_component(const GameObject *go);
void *get_component(GameObject *go, CompType type);
bool has_component(GameObject *go, CompType type);

void *go_find_component_by_type(GameObject *go, Scene *scene, CompType component_type);

#endif