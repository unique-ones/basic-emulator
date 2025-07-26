// Copyright (c) 2025 Elias Engelbert Plank

#ifndef RETRO_INPUT_H
#define RETRO_INPUT_H

typedef struct InputBuffer {
    char *data;
    ssize fill;
    ssize cursor;
    ssize capacity;
    b32 submit;
} InputBuffer;

/// Creates an input buffer
/// @param self The buffer handle
/// @param capacity The capacity of the input buffer
static void input_buffer_create(InputBuffer *self, ssize capacity);

/// Destroys the input buffer and frees its data
/// @param self The buffer handle
static void input_buffer_destroy(InputBuffer const *self);

/// Inserts a character into the input buffer
/// @param self The buffer handle
/// @param data The data
/// @return A b32ean value that indicates whether data could be emplaced
static b32 input_buffer_emplace(InputBuffer *self, char data);

/// Removes data at the cursor, reorders buffer to be continuous in memory
/// @param self The buffer handle
/// @return A b32ean value that indicates whether a char could be removed
static b32 input_buffer_remove(InputBuffer *self);

/// Advances the cursor by the specified offset
/// @param self The input buffer handle
/// @param offset The offset
static void input_buffer_advance_cursor(InputBuffer *self, ssize offset);

/// Checks if the input buffer is full
/// @return A b32ean value that indicates whether the input buffer is full or not
static b32 input_buffer_is_full(InputBuffer const *self);

#endif// RETRO_INPUT_H
