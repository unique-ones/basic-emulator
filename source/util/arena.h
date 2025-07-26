// Copyright (c) 2025 Elias Engelbert Plank

#ifndef RETRO_UTILS_ARENA_H
#define RETRO_UTILS_ARENA_H

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
    b32 temporary;
} MemoryBlock;

typedef struct MemoryArena {
    MemoryBlock *current;
    MemoryAlignment alignment;
    ReserveFunction reserve;
    ReleaseFunction release;
    u32 blocks;
    u32 total_memory;
    b32 temporary;
} MemoryArena;

/// Creates a new memory arena
/// @param spec The arena specification
/// @return Memory arena with one block
static MemoryArena arena_make(ArenaSpecification const *spec);

/// Creates an identity memory arena
/// @param alignment The alignment for the allocations
/// @return Identity memory arena with one block
///
/// @note An identity arena is an arena with malloc and free
///       as reserve/release functions
static MemoryArena arena_identity(MemoryAlignment alignment);

/// Destroys the specified memory arena
/// @param self The arena
static void arena_destroy(MemoryArena *self);

/// Allocates a block of memory in the specified arena
/// @param self The arena
/// @param size The size of the requested block
/// @return Memory
static void *arena_alloc(MemoryArena *self, usize size);

/// Begins a temporary scope where all subsequent allocations are freed after
/// calling arena_end_temporary(). Note that previous allocations are not
/// affected.
/// @param self The arena
static void arena_begin_temporary(MemoryArena *self);

/// Ends the temporary scope by freeing all memory allocations that are
/// marked as temporary.
/// @param self The arena
static void arena_end_temporary(MemoryArena *self);


#endif// RETRO_UTILS_ARENA_H
