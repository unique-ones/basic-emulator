// Copyright (c) 2025 Elias Engelbert Plank

#ifndef RETRO_UTIL_MAP_H
#define RETRO_UTIL_MAP_H

enum {
    MAP_BUCKET_COUNT = 16
};

typedef struct HashMap {
    LinkedList *buckets[MAP_BUCKET_COUNT];
} HashMap;

/// Allocates a new map instance
/// @return A new map instance
HashMap *hash_map_new(void);

/// Frees the map and its buckets
/// @param self The map handle
void hash_map_free(HashMap *self);

/// Clears the map and its buckets
/// @param self The map handle
void hash_map_clear(HashMap const *self);

/// Removes the specified key from the map
/// @param self The map handle
/// @param key The key that shall be removed
void hash_map_remove(HashMap const *self, const char *key);

/// Inserts the specified key-value pair into the map
/// @param self The map handle
/// @param key The key under which the value will be placed
/// @param value The value that shall be inserted
void hash_map_insert(HashMap const *self, const char *key, void *value);

/// Inserts the specified key-value pair into the map
/// @param self The map handle
/// @param key The key under which the value will be placed
/// @param value The value that shall be inserted
void hash_map_insert_number(HashMap const *self, u64 key, void *value);

/// Tries to find a key-value pair where the key matches with the specified entry
/// @param self The map handle
/// @param key The key for which the value shall be found
/// @return A reference to the found value or NULL
void *hash_map_find(HashMap const *self, const char *key);

/// Tries to find a key-value pair where the key matches with the specified entry
/// @param self The map handle
/// @param key The key for which the value shall be found
/// @return A reference to the found value or NULL
void *hash_map_find_number(HashMap const *self, u64 key);

/// Superfast hash function by paul hsieh
/// @cite http://www.azillionmonkeys.com/qed/hash.html
/// @author paul hsieh
/// @copyright 2010, paul hsieh
///
/// Computes hash of byte array
/// @param data byte array that shall be hashed
/// @param size size of the byte array
/// @return superfast hash
u32 hash(const char *data, usize size);


#endif// RETRO_UTIL_MAP_H
