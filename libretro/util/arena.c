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
u32 arena_alignment_size(MemoryArena *self, u32 size) {
    u32 align_distance = size % self->alignment;
    if (align_distance != 0) {
        size += align_distance;
    }
    return size;
}

/// Aligns the used offset according to arena alignment
u32 arena_alignment_offset(MemoryArena *self) {
    return arena_alignment_size(self, self->current->used);
}

MemoryBlock *arena_block_new(MemoryArena *self, u32 requested_size, bool temporary) {
    // Default block size is 4 Kb, which should be a page?
    u32 block_size = 4 * 1024;

    // At this point, the requested size is already aligned
    u32 actual_size = requested_size > block_size ? requested_size : block_size;

    MemoryBlock *blck = (MemoryBlock *) self->reserve(sizeof(MemoryBlock) + actual_size);
    blck->base = (u8 *) blck + sizeof(MemoryBlock);
    blck->size = actual_size;
    blck->used = 0;
    blck->before = NULL;
    blck->id = self->blocks++;
    blck->temporary = temporary;
    self->total_memory += block_size;
    return blck;
}

/// Creates a new memory arena
MemoryArena arena_make(ArenaSpecification *spec) {
    MemoryArena result;
    result.alignment = spec->alignment;
    result.reserve = spec->reserve;
    result.release = spec->release;
    result.blocks = 0;
    result.total_memory = 0;
    result.current = arena_block_new(&result, 0, false);
    result.temporary = false;
    return result;
}

/// Creates an identity memory arena
MemoryArena arena_identity(MemoryAlignment alignment) {
    ArenaSpecification spec;
    spec.alignment = alignment;
    spec.reserve = malloc;
    spec.release = free;
    return arena_make(&spec);
}

/// Destroys the specified memory arena
void arena_destroy(MemoryArena *self) {
    while (self->current != NULL) {
        MemoryBlock *before = self->current->before;
        // We must release the memory block itself as it is the base of the allocation
        self->release(self->current);
        self->current = before;
    }
    self->reserve = NULL;
    self->release = NULL;
    self->current = NULL;
}

static void arena_append_block(MemoryArena *self, MemoryBlock *blck) {
    blck->before = self->current;
    self->current = blck;
}

/// Allocates a block of memory in the specified arena
void *arena_alloc(MemoryArena *self, u32 size) {
    u32 aligned_size = arena_alignment_size(self, size);

    if (self->current->used + aligned_size > self->current->size) {
        MemoryBlock *blck = arena_block_new(self, aligned_size, self->temporary);
        arena_append_block(self, blck);
    }

    u32 aligned_offset = arena_alignment_offset(self);
    u8 *result = self->current->base + aligned_offset;
    self->current->used = aligned_offset + aligned_size;
    return result;
}

/// Begins a temporary scope where all subsequent allocations are freed after
/// calling arena_end_temporary(). Note that previous allocations are not
/// affected.
void arena_begin_temporary(MemoryArena *self) {
    self->temporary = true;
    MemoryBlock *blck = arena_block_new(self, 0, true);
    arena_append_block(self, blck);
}

/// Ends the temporary scope by freeing all memory allocations that are
/// marked as temporary.
void arena_end_temporary(MemoryArena *self) {
    self->temporary = false;
    for (; self->current != NULL && self->current->temporary; self->current = self->current->before) {
        self->release(self->current);
    }
}
