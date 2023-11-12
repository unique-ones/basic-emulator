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

#include "arena.h"

/// Aligns the specified size according to arena alignment
u32 arena_alignment_size(arena_t* arena, u32 size) {
    u32 align_distance = size % arena->alignment;
    if (align_distance != 0) {
        size += align_distance;
    }
    return size;
}

/// Aligns the used offset according to arena alignment
u32 arena_alignment_offset(arena_t* arena) {
    return arena_alignment_size(arena, arena->current->used);
}

block_t* arena_block_new(arena_t* arena, u32 requested_size) {
    // Default block size is 4 Kb, which should be a page?
    u32 block_size = 4 * 1024;

    // At this point, the requested size is already aligned
    u32 actual_size = requested_size > block_size ? requested_size : block_size;

    block_t* block = (block_t*) arena->reserve(sizeof(block_t) + actual_size);
    block->base = (u8*) block + sizeof(block_t);
    block->size = actual_size;
    block->used = 0;
    block->before = NULL;
    block->id = arena->blocks++;
    arena->total_memory += block_size;
    return block;
}

/// Creates a new memory arena
arena_t arena_make(arena_specification_t* spec) {
    arena_t result;
    result.alignment = spec->alignment;
    result.reserve = spec->reserve;
    result.release = spec->release;
    result.blocks = 0;
    result.total_memory = 0;
    result.current = arena_block_new(&result, 0);
    return result;
}

/// Creates an identity memory arena
arena_t arena_identity(alignment_t alignment) {
    arena_specification_t spec;
    spec.alignment = alignment;
    spec.reserve = malloc;
    spec.release = free;
    return arena_make(&spec);
}

/// Destroys the specified memory arena
void arena_destroy(arena_t* arena) {
    while (arena->current != NULL) {
        block_t* before = arena->current->before;
        // We must release the memory block itself as it is the base of the allocation
        arena->release(arena->current);
        arena->current = before;
    }
    arena->reserve = NULL;
    arena->release = NULL;
    arena->current = NULL;
}

/// Allocates a block of memory in the specified arena
void* arena_alloc(arena_t* arena, u32 size) {
    u32 aligned_size = arena_alignment_size(arena, size);

    bool back_swap = false;
    if (arena->current->used + aligned_size > arena->current->size) {
        // If the current block is less than half full, we prepend a new block
        // for the incoming allocation to the old current block, to not waste the unused memory
        block_t* block = arena_block_new(arena, aligned_size);
        if (arena->current->used < arena->current->size / 2) {
            back_swap = true;
        }
        block->before = arena->current;
        arena->current = block;
    }

    u32 aligned_offset = arena_alignment_offset(arena);
    u8* result = arena->current->base + aligned_offset;
    arena->current->used = aligned_offset + aligned_size;

    if (back_swap) {
        block_t* current = arena->current;
        block_t* previous_before = current->before->before;
        arena->current = arena->current->before;
        arena->current->before = current;
        arena->current->before->before = previous_before;
    }

    return result;
}