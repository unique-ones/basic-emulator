// Copyright (c) 2025 Elias Engelbert Plank

#ifndef RETRO_INPUT_H
#define RETRO_INPUT_H

#include "types.h"

typedef struct InputBuffer {
    char *data;
    usize fill;
    usize cursor;
    usize capacity;
    bool submit;
} InputBuffer;

/// Creates an input buffer
/// @param self The buffer handle
/// @param capacity The capacity of the input buffer
void input_buffer_create(InputBuffer *self, usize capacity);

/// Destroys the input buffer and frees its data
/// @param self The buffer handle
void input_buffer_destroy(InputBuffer *self);

/// Inserts a character into the input buffer
/// @param self The buffer handle
/// @param data The data
/// @return A boolean value that indicates whether data could be emplaced
bool input_buffer_emplace(InputBuffer *self, char data);

/// Removes data at the cursor, reorders buffer to be continuous in memory
/// @param self The buffer handle
/// @return A boolean value that indicates whether a char could be removed
bool input_buffer_remove(InputBuffer *self);

/// Advances the cursor by the specified offset
/// @param self The input buffer handle
/// @param offset The offset
void input_buffer_advance_cursor(InputBuffer *self, s64 offset);

/// Checks if the input buffer is full
/// @return A boolean value that indicates whether the input buffer is full or not
bool input_buffer_is_full(InputBuffer *self);

#endif// RETRO_INPUT_H
