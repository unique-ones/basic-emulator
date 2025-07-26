// Copyright (c) 2025 Elias Engelbert Plank

/// Aligns the specified size according to arena alignment
usize arena_alignment_size(MemoryArena const *self, usize size) {
    usize const align_distance = size % self->alignment;
    if (align_distance != 0) {
        size += align_distance;
    }
    return size;
}

/// Aligns the used offset according to arena alignment
usize arena_alignment_offset(MemoryArena const *self) {
    return arena_alignment_size(self, self->current->used);
}

MemoryBlock *arena_block_new(MemoryArena *self, usize const requested_size, bool const temporary) {
    // Default block size is 4 Kb, which should be a page?
    usize const block_size = 4 * 1024;

    // At this point, the requested size is already aligned
    usize const actual_size = requested_size > block_size ? requested_size : block_size;

    MemoryBlock *block = self->reserve(sizeof(MemoryBlock) + actual_size);
    block->base = (u8 *) block + sizeof(MemoryBlock);
    block->size = actual_size;
    block->used = 0;
    block->before = NULL;
    block->id = self->blocks++;
    block->temporary = temporary;
    self->total_memory += block_size;
    return block;
}

/// Creates a new memory arena
MemoryArena arena_make(ArenaSpecification const *spec) {
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
MemoryArena arena_identity(MemoryAlignment const alignment) {
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

static void arena_append_block(MemoryArena *self, MemoryBlock *block) {
    block->before = self->current;
    self->current = block;
}

/// Allocates a block of memory in the specified arena
void *arena_alloc(MemoryArena *self, usize const size) {
    usize const aligned_size = arena_alignment_size(self, size);

    if (self->current->used + aligned_size > self->current->size) {
        MemoryBlock *block = arena_block_new(self, aligned_size, self->temporary);
        arena_append_block(self, block);
    }

    usize const aligned_offset = arena_alignment_offset(self);
    u8 *result = self->current->base + aligned_offset;
    self->current->used = aligned_offset + aligned_size;
    return result;
}

/// Begins a temporary scope where all subsequent allocations are freed after
/// calling arena_end_temporary(). Note that previous allocations are not
/// affected.
void arena_begin_temporary(MemoryArena *self) {
    self->temporary = true;
    MemoryBlock *block = arena_block_new(self, 0, true);
    arena_append_block(self, block);
}

/// Ends the temporary scope by freeing all memory allocations that are
/// marked as temporary.
void arena_end_temporary(MemoryArena *self) {
    self->temporary = false;
    for (; self->current != NULL && self->current->temporary; self->current = self->current->before) {
        self->release(self->current);
    }
}
