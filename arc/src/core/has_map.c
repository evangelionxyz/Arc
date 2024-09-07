#include "hash_map.h"

HashMap *create_hash_map(i32 bucket_count, i32(*hash_func)(void *, i32 *count), i32(*key_compare)(void *, void *))
{
    HashMap *map = (HashMap *)malloc(sizeof(HashMap));
    if (map == NULL)
    {
        perror("Failed to allocate memory for HashMap");
        exit(EXIT_FAILURE);
    }

    map->buckets = (HashNode **)malloc(sizeof(HashNode *) * bucket_count);
    if (map->buckets == NULL)
    {
        perror("Failed to allocate memory for buckets");
        free(map);  // Free the already allocated map memory
        exit(EXIT_FAILURE);
    }

    map->bucket_count = bucket_count;
    map->hash_func = hash_func;
    map->key_compare = key_compare;

    for (i32 i = 0; i < bucket_count; ++i)
    {
        map->buckets[i] = NULL;
    }

    return map;
}

void hash_map_insert(HashMap *map, void *key, void *value)
{
    i32 index = map->hash_func(key, &map->bucket_count);
    HashNode *node = map->buckets[index];

    while (node != NULL)
    {
        if (map->key_compare(node->key, key))
        {
            // update the value
            node->value = value;
            return;
        }
        node = node->next;
    }

    // create a new node and insert it
    HashNode *new_node = (HashNode *)malloc(sizeof(HashNode));
    if (new_node == NULL)
    {
        perror("Failed to insert new node to hash map");
        exit(EXIT_FAILURE);
    }

    new_node->key = key;
    new_node->value = value;
    new_node->next = map->buckets[index]; // insert at the head of the list
    map->buckets[index] = new_node;
}

void *hash_map_find(HashMap *map, void *key)
{
    i32 index = map->hash_func(key, &map->bucket_count);
    HashNode *node = map->buckets[index];

    while (node != NULL)
    {
        if (map->key_compare(node->key, key))
            return node->value;
        node = node->next;
    }

    return NULL;
}

void hash_map_free(HashMap *map)
{
    for (i32 i = 0; i < map->bucket_count; ++i)
    {
        HashNode *node = map->buckets[i];
        while (node != NULL)
        {
            HashNode *temp = node;
            node = node->next;
            free(temp);
        }
    }

    free(map->buckets);
    free(map);
}
