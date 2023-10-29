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

typedef struct map_entry {
    const char* key;
    void* data;
} map_entry_t;

map_t* map_new(void) {
    map_t* self = (map_t*) malloc(sizeof(map_t));
    for (u32 i = 0; i < MAP_BUCKET_COUNT; ++i) {
        self->buckets[i] = list_new();
    }
    return self;
}

void map_free(map_t* self) {
    for (u32 i = 0; i < MAP_BUCKET_COUNT; ++i) {
        list_t* bucket = self->buckets[i];
        node_t* it = bucket->head;
        while (it != NULL && it->next != NULL) {
            map_entry_t* entry = it->data;
            free(entry);
            it = it->next;
        }
        list_free(bucket);
    }
    free(self);
}

static bool map_entry_equal(const void* a, const void* b) {
    map_entry_t* first = (map_entry_t*) a;
    map_entry_t* second = (map_entry_t*) b;
    return strcmp(first->key, second->key) == 0;
}

void map_insert(map_t* self, const char* key, void* value) {
    u32 bucket_index = hash(key, strlen(key)) % MAP_BUCKET_COUNT;
    list_t* bucket = self->buckets[bucket_index];

    map_entry_t* data = (map_entry_t*) malloc(sizeof(map_entry_t));
    data->key = key;
    data->data = value;

    node_t* find_result = list_find(bucket, data, map_entry_equal);
    if (find_result) {
        find_result->data = data;
    } else {
        list_append(bucket, data);
    }
}

void* map_find(map_t* self, const char* key) {
    u32 bucket_index = hash(key, strlen(key)) % MAP_BUCKET_COUNT;
    list_t* bucket = self->buckets[bucket_index];

    map_entry_t find_entry = { key, NULL };
    node_t* find_result = list_find(bucket, &find_entry, map_entry_equal);
    if (find_result) {
        map_entry_t* entry = find_result->data;
        return entry->data;
    } else {
        return NULL;
    }
}

static u32 hash_get16bits(const char* data) {
    return (((u32) (data[1])) << 8) + (u32) data[0];
}

u32 hash(const char* data, u32 size) {
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
