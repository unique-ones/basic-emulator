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

#ifndef RETRO_UTIL_MAP_H
#define RETRO_UTIL_MAP_H

#include "list.h"

enum { MAP_BUCKET_COUNT = 16 };

typedef struct map {
    list_t* buckets[MAP_BUCKET_COUNT];
} map_t;

/// Allocates a new map instance
/// @return A new map instance
map_t* map_new(void);

/// Frees the map and its buckets
/// @param self The map handle
void map_free(map_t* self);

/// Inserts the specified key-value pair into the map
/// @param self The map handle
/// @param key The key under which the value will be placed
/// @param value The value that shall be inserted
void map_insert(map_t* self, const char* key, void* value);

/// Tries to find a key-value pair where the key matches with the specified entry
/// @param self The map handle
/// @param key The key for which the value shall be found
/// @return A reference to the found value or NULL
void* map_find(map_t* self, const char* key);

/// Super fast hash function by paul hsieh
/// @cite http://www.azillionmonkeys.com/qed/hash.html
/// @author paul hsieh
/// @copyright 2010, paul hsieh
///
/// Computes hash of byte array
/// @param data byte array that shall be hashed
/// @param size size of the byte array
/// @return super fast hash
u32 hash(const char* data, u32 size);

#endif// RETRO_UTIL_MAP_H
