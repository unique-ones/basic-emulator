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

#ifndef RETRO_UTILS_ARENA_H
#define RETRO_UTILS_ARENA_H

#include "types.h"

typedef void* (*reserve_func_t)(u64);
typedef void (*release_func_t)(void*);

typedef enum alignment { ALIGNMENT1 = 1, ALIGNMENT4 = 4, ALIGNMENT8 = 8 } alignment_t;

typedef struct arena_specification {
    alignment_t alignment;
    reserve_func_t reserve;
    release_func_t release;
} arena_specification_t;

typedef struct block block_t;

typedef struct block {
    u32 size;
    u32 used;
    u8* base;
    block_t* before;
    u32 id;
} block_t;

typedef struct arena {
    block_t* current;
    alignment_t alignment;
    reserve_func_t reserve;
    release_func_t release;
    u32 blocks;
    u32 total_memory;
} arena_t;

/// Creates a new memory arena
/// @param arena The arena
/// @param spec The arena specification
/// @return Memory arena with one block
arena_t arena_make(arena_specification_t* spec);

/// Creates an identity memory arena
/// @param alignment The alignment for the allocations
/// @return Identity memory arena with one block
///
/// @note An identity arena is an arena with malloc and free
///       as reserve/release functions
arena_t arena_identity(alignment_t alignment);

/// Destroys the specified memory arena
/// @param arena The arena
void arena_destroy(arena_t* arena);

/// Allocates a block of memory in the specified arena
/// @param arena The arena
/// @param size The size of the requested block
/// @return Memory
void* arena_alloc(arena_t* arena, u32 size);


#endif// RETRO_UTILS_ARENA_H