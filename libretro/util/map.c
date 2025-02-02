//
// MIT License
//
// Copyright (c) 2023 Elias Engelbert Plank
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <stdlib.h>
#include <string.h>

#include "map.h"

typedef enum map_entry_key_type {
    KEY_TYPE_STRING,
    KEY_TYPE_NUMBER
} map_entry_key_type_t;

typedef struct map_entry {
    map_entry_key_type_t type;
    union {
        const char *key;
        u32 key_number;
    };
    void *data;
} map_entry_t;

/// Allocates a new map instance
map_t *map_new(void) {
    map_t *self = (map_t *) malloc(sizeof(map_t));
    for (u32 i = 0; i < MAP_BUCKET_COUNT; ++i) {
        self->buckets[i] = list_new();
    }
    return self;
}

/// Frees the map and its buckets
void map_free(map_t *self) {
    for (u32 i = 0; i < MAP_BUCKET_COUNT; ++i) {
        list_t *bucket = self->buckets[i];
        node_t *it = bucket->head;
        while (it != NULL && it->next != NULL) {
            map_entry_t *entry = it->data;
            free(entry);
            it = it->next;
        }
        list_free(bucket);
    }
    free(self);
}

/// Clears the map and its buckets
/// @param self The map handle
void map_clear(map_t *self) {
    for (u32 i = 0; i < MAP_BUCKET_COUNT; ++i) {
        list_clear(self->buckets[i]);
    }
}

/// Checks if two map entries are equal
static bool map_entry_equal(const void *a, const void *b) {
    const map_entry_t *first = (const map_entry_t *) a;
    const map_entry_t *second = (const map_entry_t *) b;
    if (first->type != second->type) {
        return false;
    }
    if (first->type == KEY_TYPE_NUMBER && first->key_number != second->key_number) {
        return false;
    }
    return strcmp(first->key, second->key) == 0;
}

/// Removes the specified key from the map
void map_remove(map_t *self, const char *key) {
    u32 bucket_index = hash(key, (u32) strlen(key)) % MAP_BUCKET_COUNT;
    list_t *bucket = self->buckets[bucket_index];

    map_entry_t entry = { .type = KEY_TYPE_STRING, .key = key, .data = NULL };
    list_remove(bucket, &entry, map_entry_equal);
}

/// Inserts the specified key-value pair into the map
void map_insert(map_t *self, const char *key, void *value) {
    u32 bucket_index = hash(key, (u32) strlen(key)) % MAP_BUCKET_COUNT;
    list_t *bucket = self->buckets[bucket_index];

    map_entry_t *data = (map_entry_t *) malloc(sizeof(map_entry_t));
    data->type = KEY_TYPE_STRING;
    data->key = key;
    data->data = value;

    node_t *find_result = list_find(bucket, data, map_entry_equal);
    if (find_result) {
        find_result->data = data;
    } else {
        list_append(bucket, data);
    }
}

/// Inserts the specified key-value pair into the map
void map_insert_number(map_t *self, u32 key, void *value) {
    u32 bucket_index = key % MAP_BUCKET_COUNT;
    list_t *bucket = self->buckets[bucket_index];

    map_entry_t *data = (map_entry_t *) malloc(sizeof(map_entry_t));
    data->type = KEY_TYPE_NUMBER;
    data->key_number = key;
    data->data = value;

    node_t *find_result = list_find(bucket, data, map_entry_equal);
    if (find_result) {
        find_result->data = data;
    } else {
        list_append(bucket, data);
    }
}

/// Tries to find a key-value pair where the key matches with the specified entry
void *map_find(map_t *self, const char *key) {
    u32 bucket_index = hash(key, strlen(key)) % MAP_BUCKET_COUNT;
    list_t *bucket = self->buckets[bucket_index];

    map_entry_t find_entry = { KEY_TYPE_STRING, { .key = key }, NULL };
    node_t *find_result = list_find(bucket, &find_entry, map_entry_equal);
    if (find_result) {
        map_entry_t *entry = find_result->data;
        return entry->data;
    } else {
        return NULL;
    }
}

/// Tries to find a key-value pair where the key matches with the specified entry
void *map_find_number(map_t *self, u32 key) {
    u32 bucket_index = key % MAP_BUCKET_COUNT;
    list_t *bucket = self->buckets[bucket_index];

    map_entry_t find_entry = { KEY_TYPE_NUMBER, { .key_number = key }, NULL };
    node_t *find_result = list_find(bucket, &find_entry, map_entry_equal);
    if (find_result) {
        map_entry_t *entry = find_result->data;
        return entry->data;
    } else {
        return NULL;
    }
}

/// Retrieves the first 16 bits of the data string
static u32 hash_get16bits(const char *data) {
    return (((u32) (data[1])) << 8) + (u32) data[0];
}

/// Computes hash of byte array
u32 hash(const char *data, u32 size) {
    if (size == 0 || data == NULL) {
        return 0;
    }

    u32 result = size;
    u32 remainder = size & 3;
    size >>= 2;

    // main loop
    for (u32 temp = 0; size > 0; size--) {
        result += hash_get16bits(data);
        temp = (u32) (hash_get16bits(data + 2) << 11) ^ result;
        result = (result << 16) ^ temp;
        data += 2 * sizeof(u16);
        result += result >> 11;
    }

    // handle end cases
    switch (remainder) {
        case 3:
            result += hash_get16bits(data);
            result ^= result << 16;
            result ^= (u32) (((signed char) data[sizeof(u16)]) << 18);
            result += result >> 11;
            break;
        case 2:
            result += hash_get16bits(data);
            result ^= result << 11;
            result += result >> 17;
            break;
        case 1:
            result += (u32) ((signed char) *data);
            result ^= result << 10;
            result += result >> 1;
        default:
            break;
    }

    // force avalanching of final 127 bites
    result ^= result << 3;
    result += result >> 5;
    result ^= result << 4;
    result += result >> 17;
    result ^= result << 25;
    result += result >> 6;

    return result;
}
