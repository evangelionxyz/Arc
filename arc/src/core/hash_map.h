#ifndef HASH_MAP
#define HASH_MAP

#include <stdio.h>
#include <stdlib.h>

#include "types.h"

typedef struct HashNode
{
    void *key;
    void *value;
    struct HashNode *next;
}HashNode;

typedef struct HashMap
{
    HashNode **buckets;
    i32 bucket_count;
    i32 (*hash_func)(void *key, i32 *count);
    i32 (*key_compare)(void *key1, void *key2);
} HashMap;

HashMap *create_hash_map(i32 bucket_count, i32(*hash_func)(void *, i32 *count), i32(*key_compare)(void *, void *));
void hash_map_insert(HashMap *map, void *key, void *value);
void *hash_map_find(HashMap *map, void *key);
void hash_map_free(HashMap *map);


#endif