#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "raylib.h"
#include "core/darray.h"
#include "core/types.h"
#include "components.h"

#define MAX_COMPONENT 10
static const INVALID_COMPONENT_ID = (u32)-1;

typedef struct Scene Scene;
typedef struct GameObject {
    u32 component_ids[MAX_COMPONENT];
    u8 component_count;
    size_t id;
    const char *name;
} GameObject;

typedef struct GoRegistry {
    DynamicArray *game_objects;
    DynamicArray *components;
} GoRegistry;

GameObject *create_game_object(const char *name, const Scene *scene, const Vector3 position, const Vector3 scale, const Vector3 rotation);
GameObject *get_game_object_by_name(const char *name, const Scene *scene);
void destroy_game_object(GameObject *go, Scene *scene);

void add_component(GameObject *go, void *component, const Scene *scene);
void remove_component(GameObject *go, CompType type, Scene *scene);

TransformComponent *get_transform_component(const GameObject *go, const Scene *scene);
void *get_component(const GameObject *go, CompType type, const Scene *scene);
bool has_component(const GameObject *go, CompType type, const Scene *scene);

#endif