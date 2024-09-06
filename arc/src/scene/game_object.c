#include "game_object.h"
#include "scene.h"

#include <assert.h>

static void *find_component_by_id(const Scene *scene, const u32 component_id)
{
    if (component_id >= scene->registry.components->size)
        return NULL;

    return scene->registry.components->data[component_id];
}

static void *find_component_by_type(const Scene *scene, const GameObject game_object, const CompType component_type)
{
    for (u8 i = 0; i < game_object.component_count; ++i)
    {
        const u32 id = game_object.component_ids[i];
        Component *comp = scene->registry.components->data[id];
        if (comp != NULL && comp->type == component_type)
            return comp;
    }
    return NULL;
}

GameObject *create_game_object(const char *name, Scene *scene, const Vector3 position, const Vector3 scale, const Vector3 rotation)
{
    if (scene == NULL)
        return NULL;

    GameObject *go = malloc(sizeof(GameObject));

    if (go == NULL)
        return NULL;

    go->component_count = 0;
    go->id              = scene->registry.game_objects->size;
    go->name            = name;
    go->scene           = scene;

    // set default value for all of Components slot
    for (u8 i = 0; i < MAX_COMPONENT; ++i)
    {
        go->component_ids[i] = INVALID_COMPONENT_ID;
    }

    TransformComponent *tc = create_component(T_TRANSFORM);
    tc->base.id            = scene->registry.components->size;
    tc->translation        = position;
    tc->scale              = scale;
    tc->rotation           = rotation;

    add_component(go, tc, scene);
    da_push_back(scene->registry.game_objects, go);
    return go;
}

GameObject *get_game_object_by_name(const char *name, const Scene *scene)
{
    for (u8 i = 0; i < scene->registry.game_objects->size; ++i)
    {
        GameObject *go = scene->registry.game_objects->data[i];
        if (strcmp(go->name, name) == 0)
            return go;
    }
    return NULL;
}

void destroy_game_object(GameObject *go, Scene *scene)
{
    if (go == NULL || scene == NULL) return;

    for (u8 i = 0; i < go->component_count; ++i)
    {
        // remove COMPONENT from SCENE REGISTRY
        const u32 id = go->component_ids[i];
        Component *comp = scene->registry.components->data[id];
        if (comp != NULL)
            remove_component(go, comp->type, scene);
    }

    // remove GAME OBJECT from SCENE REGISTRY
    da_remove_element(scene->registry.game_objects, go->id);
}

void add_component(GameObject *go, void *component, const Scene *scene)
{
    // set component registry id base on component size
    const u32 comp_reg_id = (u32)scene->registry.components->size;
    Component *comp = component;

    comp->id = comp_reg_id;
    go->component_ids[go->component_count] = comp->id;
    go->component_count++;

    da_push_back(scene->registry.components, component);
}

void remove_component(GameObject *go, const CompType type, Scene *scene)
{
    Component *comp = find_component_by_type(scene, *go, type);
    if (comp != NULL)
    {
        if (type == T_SPRITE)
        {
            const SpriteComponent *sprite = (SpriteComponent *)comp;
            UnloadTexture(sprite->texture);
        }
        else if (type == T_BOX_COLLIDER)
        {
            const BoxCollider2DComponent *bc = (BoxCollider2DComponent *)comp;
            if (b2Body_IsValid(bc->body_id))
                b2DestroyBody(bc->body_id);
        }

        // remove the component from registry
        da_remove_element(scene->registry.components, comp->id);

        // remove the component from game object
         for (u8 i = 0; i < go->component_count; ++i)
         {
            if (go->component_ids[i] == comp->id)
            {
                // shift the component IDs to fill the gap
                for (u8 j = i; j < go->component_count; ++j)
                    go->component_ids[j] = go->component_ids[j + 1];

                // clear the last component ID slot
                go->component_ids[go->component_count - 1] = INVALID_COMPONENT_ID;
                go->component_count--;
                break; // exit the loop once the component is removed
            }
         }
    }
}

TransformComponent *get_transform_component(const GameObject *go, const Scene *scene)
{
    if (go == NULL || scene == NULL)
        return NULL;

    // transform is always the first element
    const u32 component_id = go->component_ids[0];
    return scene->registry.components->data[component_id];
}

void *get_component(const GameObject *go, const CompType type, const Scene *scene)
{
    for (u8 i = 0; i < go->component_count; ++i)
    {
        const u32 component_id = go->component_ids[i];
        Component *comp = find_component_by_id(scene, component_id);
        if (comp != NULL && comp->type == type)
            return comp;
    }

    return NULL;
}

bool has_component(const GameObject *go, CompType type, const Scene *scene)
{
    for (u8 i = 0; i < go->component_count; ++i)
    {
        const u32 component_id = go->component_ids[i];
        const Component *comp = (Component *)find_component_by_id(scene, component_id);

        if (comp != NULL && comp->type == type)
            return true;
    }

    return false;
}