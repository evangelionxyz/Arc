#include "game_object.h"

#include <assert.h>

static void *find_component_by_id(const GoRegistry *registry, const u32 component_id)
{
    if (component_id >= registry->components->size)
        return NULL;

    return registry->components->data[component_id];
}

static void *find_component_by_type(const GoRegistry *registry, const GameObject game_object, const CompType component_type)
{
    for (u8 i = 0; i < game_object.component_count; ++i)
    {
        Component *comp = registry->components->data[i];
        if (comp != NULL && comp->type == component_type)
            return comp;
    }
    return NULL;
}

GameObject *create_game_object(GoRegistry *registry, const Vector2 position, const Vector2 scale)
{
    GameObject *go = malloc(sizeof(GameObject));
    go->id = registry->game_objects->size;
    go->component_count = 0;

    for (u8 i = 0; i < MAX_COMPONENT; ++i)
        go->component_ids[i] = (u32)-1;

    TransformComponent *tc = create_component(TypeTransform);
    tc->base.id = registry->components->size;
    tc->translation = position;
    tc->scale = scale;
    tc->angle = 0.0f;

    add_component(go, tc, registry);

    da_push_back(registry->game_objects, go);
    return go;
}

void destroy_game_object(const GoRegistry *registry, const GameObject *go)
{
    for (u8 i = 0; i < go->component_count; ++i)
        da_remove_element(registry->components, go->component_ids[i]);

    da_remove_element(registry->game_objects, go->id);
}

void add_component(GameObject *go, void *component, const GoRegistry *registry)
{
    // set component registry id base on component size
    const size_t comp_reg_id = registry->components->size;
    Component *comp = component;

    comp->id = comp_reg_id;
    go->component_ids[go->component_count] = comp->id;
    go->component_count++;

    da_push_back(registry->components, component);
}

void remove_component(GameObject *go, const CompType type, const GoRegistry *registry)
{
    Component *comp = find_component_by_type(registry, *go, type);
    if (comp != NULL)
    {
        // remove the component from registry
        da_remove_element(registry->components, comp->id);

        // remove the component from game object
        if (comp->id < go->component_count)
        {
            for (u8 i = comp->id; i < go->component_count - 1; ++i)
                go->component_ids[i] = go->component_ids[i + 1];
        }

        free(comp);
        go->component_count--;
    }
}

void *get_component(const GameObject *go, const CompType type, const GoRegistry *registry)
{
    for (u8 i = 0; i < go->component_count; ++i)
    {
        const u32 component_id = go->component_ids[i];
        Component *comp = find_component_by_id(registry, component_id);
        if (comp != NULL && comp->type == type)
            return comp;
    }

    return NULL;
}

bool has_component(const GameObject *go, CompType type, const GoRegistry *registry)
{
    for (u8 i = 0; i < go->component_count; ++i)
    {
        const u32 component_id = go->component_ids[i];
        const Component *comp = (Component *)find_component_by_id(registry, component_id);
        if (comp != NULL && comp->type == type)
        {
            return true;
        }
    }

    return false;
}