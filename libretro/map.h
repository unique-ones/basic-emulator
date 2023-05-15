#ifndef RETRO_COLLECTIONS_MAP_H
#define RETRO_COLLECTIONS_MAP_H

#include "list.h"

#define MAP_BUCKET_COUNT 16

typedef struct map {
    list_t* buckets[MAP_BUCKET_COUNT];
} map_t;

/**
 * @brief allocates a new map instance
 * @return map instance
 */
map_t* map_new();

/**
 * @brief frees the map instance and its buckets
 * @param self reference to map instance
 */
void map_free(map_t* self);

/**
 * @brief inserts the specified key-value pair into the map
 * @param self reference to map instance
 * @param key key under which the value will be placed
 * @param value value that shall be inserted
 */
void map_insert(map_t* self, const char* key, void* value);

/**
 * @brief tries to find a key-value pair where the key matches with the specified entry
 * @param self reference to map instance
 * @param key key for which the value shall be found
 * @return referene to the found value
 */
void* map_find(map_t* self, const char* key);

/**
 * Super fast hash function by paul hsieh
 * @cite http://www.azillionmonkeys.com/qed/hash.html
 * @author paul hsieh
 * @copyright 2010, paul hsieh
 *
 * @brief compute hash of byte array
 * @param data byte array that shall be hashed
 * @param size size of the byte array
 * @return super fast hash
 */
u32 hash(const char* data, u32 size);

#endif// RETRO_COLLECTIONS_MAP_H
