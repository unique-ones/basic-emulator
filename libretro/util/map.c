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

typedef enum hash_map_entry_key_type {
    KEY_TYPE_STRING,
    KEY_TYPE_NUMBER
} hash_map_entry_key_type_t;

typedef struct hash_map_entry {
    hash_map_entry_key_type_t type;
    union {
        const char *key;
        u64 key_number;
    };
    void *data;
} hash_map_entry_t;

/// Allocates a new map instance
HashMap *hash_map_new(void) {
    HashMap *self = (HashMap *) malloc(sizeof(HashMap));
    for (usize i = 0; i < MAP_BUCKET_COUNT; ++i) {
        self->buckets[i] = linked_list_new();
    }
    return self;
}

/// Frees the map and its buckets
void hash_map_free(HashMap *self) {
    for (usize i = 0; i < MAP_BUCKET_COUNT; ++i) {
        LinkedList *bucket = self->buckets[i];
        ListNode const *it = bucket->head;
        while (it != NULL && it->next != NULL) {
            hash_map_entry_t *entry = it->data;
            free(entry);
            it = it->next;
        }
        linked_list_free(bucket);
    }
    free(self);
}

/// Clears the map and its buckets
void hash_map_clear(HashMap const *self) {
    for (usize i = 0; i < MAP_BUCKET_COUNT; ++i) {
        linked_list_clear(self->buckets[i]);
    }
}

/// Checks if two map entries are equal
static bool hash_map_entry_equal(void const *a, void const *b) {
    hash_map_entry_t const *first = a;
    hash_map_entry_t const *second = b;
    if (first->type != second->type) {
        return false;
    }
    if (first->type == KEY_TYPE_NUMBER && first->key_number != second->key_number) {
        return false;
    }
    return strcmp(first->key, second->key) == 0;
}

/// Removes the specified key from the map
void hash_map_remove(HashMap const *self, char const *key) {
    usize const bucket_index = hash(key, strlen(key)) % MAP_BUCKET_COUNT;
    LinkedList *bucket = self->buckets[bucket_index];

    hash_map_entry_t entry = { .type = KEY_TYPE_STRING, .key = key, .data = NULL };
    linked_list_remove(bucket, &entry, hash_map_entry_equal);
}

/// Inserts the specified key-value pair into the map
void hash_map_insert(HashMap const *self, char const *key, void *value) {
    usize const bucket_index = hash(key, strlen(key)) % MAP_BUCKET_COUNT;
    LinkedList *bucket = self->buckets[bucket_index];

    hash_map_entry_t *data = malloc(sizeof(hash_map_entry_t));
    data->type = KEY_TYPE_STRING;
    data->key = key;
    data->data = value;

    ListNode *find_result = linked_list_find(bucket, data, hash_map_entry_equal);
    if (find_result) {
        find_result->data = data;
    } else {
        linked_list_append(bucket, data);
    }
}

/// Inserts the specified key-value pair into the map
void hash_map_insert_number(HashMap const *self, u64 const key, void *value) {
    usize const bucket_index = key % MAP_BUCKET_COUNT;
    LinkedList *bucket = self->buckets[bucket_index];

    hash_map_entry_t *data = malloc(sizeof(hash_map_entry_t));
    data->type = KEY_TYPE_NUMBER;
    data->key_number = key;
    data->data = value;

    ListNode *find_result = linked_list_find(bucket, data, hash_map_entry_equal);
    if (find_result) {
        find_result->data = data;
    } else {
        linked_list_append(bucket, data);
    }
}

/// Tries to find a key-value pair where the key matches with the specified entry
void *hash_map_find(HashMap const *self, char const *key) {
    usize const bucket_index = hash(key, strlen(key)) % MAP_BUCKET_COUNT;
    LinkedList const *bucket = self->buckets[bucket_index];

    hash_map_entry_t const find_entry = { KEY_TYPE_STRING, { .key = key }, NULL };
    ListNode const *find_result = linked_list_find(bucket, &find_entry, hash_map_entry_equal);
    if (find_result) {
        hash_map_entry_t *entry = find_result->data;
        return entry->data;
    }
    return NULL;
}

/// Tries to find a key-value pair where the key matches with the specified entry
void *hash_map_find_number(HashMap const *self, u64 const key) {
    usize const bucket_index = key % MAP_BUCKET_COUNT;
    LinkedList const *bucket = self->buckets[bucket_index];

    hash_map_entry_t const find_entry = { KEY_TYPE_NUMBER, { .key_number = key }, NULL };
    ListNode const *find_result = linked_list_find(bucket, &find_entry, hash_map_entry_equal);
    if (find_result) {
        hash_map_entry_t const *entry = find_result->data;
        return entry->data;
    }
    return NULL;
}

/// Retrieves the first 16 bits of the data string
static u32 hash_get16bits(const char *data) {
    return (((u32) (data[1])) << 8) + (u32) data[0];
}

/// Computes hash of byte array
u32 hash(const char *data, usize size) {
    if (size == 0 || data == NULL) {
        return 0;
    }

    u32 result = size;
    u32 const remainder = size & 3;
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

    // force avalanche of final 127 bites
    result ^= result << 3;
    result += result >> 5;
    result ^= result << 4;
    result += result >> 17;
    result ^= result << 25;
    result += result >> 6;

    return result;
}
