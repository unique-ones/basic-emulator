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

#include "core/types.h"

typedef void *(*ReserveFunction)(usize);
typedef void (*ReleaseFunction)(void *);

typedef enum MemoryAlignment {
    ALIGNMENT1 = 1,
    ALIGNMENT4 = 4,
    ALIGNMENT8 = 8
} MemoryAlignment;

typedef struct ArenaSpecification {
    MemoryAlignment alignment;
    ReserveFunction reserve;
    ReleaseFunction release;
} ArenaSpecification;

typedef struct MemoryBlock MemoryBlock;

typedef struct MemoryBlock {
    usize size;
    usize used;
    u8 *base;
    MemoryBlock *before;
    usize id;
    bool temporary;
} MemoryBlock;

typedef struct MemoryArena {
    MemoryBlock *current;
    MemoryAlignment alignment;
    ReserveFunction reserve;
    ReleaseFunction release;
    u32 blocks;
    u32 total_memory;
    bool temporary;
} MemoryArena;

/// Creates a new memory arena
/// @param spec The arena specification
/// @return Memory arena with one block
MemoryArena arena_make(ArenaSpecification const *spec);

/// Creates an identity memory arena
/// @param alignment The alignment for the allocations
/// @return Identity memory arena with one block
///
/// @note An identity arena is an arena with malloc and free
///       as reserve/release functions
MemoryArena arena_identity(MemoryAlignment alignment);

/// Destroys the specified memory arena
/// @param self The arena
void arena_destroy(MemoryArena *self);

/// Allocates a block of memory in the specified arena
/// @param self The arena
/// @param size The size of the requested block
/// @return Memory
void *arena_alloc(MemoryArena *self, usize size);

/// Begins a temporary scope where all subsequent allocations are freed after
/// calling arena_end_temporary(). Note that previous allocations are not
/// affected.
/// @param self The arena
void arena_begin_temporary(MemoryArena *self);

/// Ends the temporary scope by freeing all memory allocations that are
/// marked as temporary.
/// @param self The arena
void arena_end_temporary(MemoryArena *self);


#endif// RETRO_UTILS_ARENA_H
