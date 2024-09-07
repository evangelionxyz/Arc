#include "game_object.h"
#include "scene.h"

GameObject *create_game_object(const char *name, Scene *scene, const Vector3 position, const Vector3 scale, const Vector3 rotation)
{
    if (scene == NULL)
    {
        return NULL;
    }

    GameObject *go = malloc(sizeof(GameObject));

    if (go == NULL)
    {
        return NULL;
    }

    go->component_count = 0;
    go->id              = scene->registry.game_objects->size; // set id based on object count
    go->name            = name;
    go->scene           = scene; // store the scene

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

    add_component(go, tc);
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

static void reassign_ids(Scene *scene)
{
    // reassign components
    for (size_t i = 0; i < scene->registry.components->size; ++i)
    {
        Component *comp = (Component *)scene->registry.components->data[i];
        comp->id = i;
    }

    // re assign game object IDs and component IDs
    size_t current_component = 0;
    for (size_t i = 0; i < scene->registry.game_objects->size; ++i)
    {
        GameObject *go = (GameObject *)scene->registry.game_objects->data[i];
        go->id = i;
        for (size_t j = 0; j < go->component_count; ++j)
            go->component_ids[j] = current_component++;
    }
}

void destroy_game_object(GameObject *go, Scene *scene)
{
    if (go == NULL || scene == NULL) return;

    // Remove all components of the game object
    size_t go_id = go->id;

    while (go->component_count > 0)
    {
        // remove from behind 
        size_t comp_id = go->component_ids[go->component_count - 1];
        Component *comp = scene->registry.components->data[comp_id];
        if (comp->type == T_SPRITE)
        {
            const SpriteComponent *sprite = (SpriteComponent *)comp;
            if (sprite->texture.id != -842150451)
                UnloadTexture(sprite->texture);
        }
        else if (comp->type == T_BOX_COLLIDER_2D)
        {
            const BoxCollider2D *bc = (BoxCollider2D *)comp;
            b2DestroyShape(bc->shape_id);
            b2DestroyBody(bc->body_id);
        }

        for (size_t i = 0; i < scene->registry.components->size; ++i)
        {
            Component *comp = (Component *)scene->registry.components->data[i];
            if (comp->id == comp_id)
            {
                da_remove_element(scene->registry.components, i);
                break;
            }
        }

        go->component_count--;
    }

    // remove GAME OBJECT from SCENE REGISTRY
    for (size_t i = 0; i < scene->registry.game_objects->size; ++i)
    {
        GameObject *game_object = (GameObject *)scene->registry.game_objects->data[i];
        if (game_object->id == go_id)
        {
            da_remove_element(scene->registry.game_objects, i);
            break;
        }
    }

    // reset game object component ids and id
    memset(go->component_ids, 0, sizeof(go->component_ids));
    go->component_count = 0;
    go->id = 0;
    go->name = NULL;
    go->scene = NULL;

    // reassign IDs after removal
    reassign_ids(scene);
}

void *add_component(GameObject *go, void *component)
{
    if (component == NULL)
        return NULL;

    // set component registry id base on component size
    const size_t reg_id = go->scene->registry.components->size;

    Component *comp = component;

    // store the scene
    comp->scene = go->scene;

    comp->id = reg_id;
    go->component_ids[go->component_count] = reg_id;
    go->component_count++;

    da_push_back(go->scene->registry.components, component);

    return component;
}

void *remove_component(GameObject *go, const CompType type)
{
    Component *comp = go_find_component_by_type(go, go->scene, type);
    if (comp == NULL)
        return NULL;

    size_t comp_id = comp->id;
    
    if (comp->type == T_SPRITE)
    {
        const SpriteComponent *sprite = (SpriteComponent *)comp;
        if (sprite->texture.id != -842150451)
            UnloadTexture(sprite->texture);
    }
    else if (comp->type == T_BOX_COLLIDER_2D)
    {
        const BoxCollider2D *bc = (BoxCollider2D *)comp;
        b2DestroyShape(bc->shape_id);
        b2DestroyBody(bc->body_id);
    }

    for (size_t i = 0; i < go->scene->registry.components->size; ++i)
    {
        Component *comp = (Component *)go->scene->registry.components->data[i];
        if (comp->id == comp_id)
            return da_remove_element(go->scene->registry.components, i);
    }

    return NULL;
}

TransformComponent *get_transform_component(const GameObject *go)
{
    if (go == NULL) return NULL;

    // transform is always the first element
    const u32 component_id = go->component_ids[0];
    return go->scene->registry.components->data[component_id];
}

void *get_component(GameObject *go, const CompType type)
{
    for (u8 i = 0; i < go->component_count; ++i)
    {
        const size_t id = go->component_ids[i];
        Component *comp = go->scene->registry.components->data[id];

        if (comp == NULL)
            return NULL;

        if (comp->type == type)
            return comp;
    }

    return NULL;
}

bool has_component(GameObject *go, CompType type)
{
    for (u8 i = 0; i < go->component_count; ++i)
    {
        const size_t id = go->component_ids[i];
        const Component *comp = go->scene->registry.components->data[id];
        if (comp == NULL)
            return false;

        if (comp->type == type)
            return true;
    }

    return false;
}


void *go_find_component_by_type(GameObject *go, Scene *scene, CompType component_type)
{
    if (scene == NULL) return NULL;

    for (u8 i = 0; i < go->component_count; ++i)
    {
        const u32 id = go->component_ids[i];
        Component *comp = scene->registry.components->data[id];
        if (comp != NULL && comp->type == component_type)
            return comp;
    }
    return NULL;
}