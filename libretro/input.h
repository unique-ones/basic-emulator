//
// MIT License
//
// Copyright (c) 2024 Elias Engelbert Plank
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

#ifndef RETRO_INPUT_H
#define RETRO_INPUT_H

#include "types.h"

typedef struct InputBuffer {
    char *data;
    s32 fill;
    s32 cursor;
    u32 capacity;
    bool submit;
} InputBuffer;

/// Creates an input buffer
/// @param self The buffer handle
/// @param capacity The capacity of the input buffer
void input_buffer_create(InputBuffer *self, u32 capacity);

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
void input_buffer_advance_cursor(InputBuffer *self, s32 offset);

/// Checks if the input buffer is full
/// @return A boolean value that indicates whether the input buffer is full or not
bool input_buffer_is_full(InputBuffer *self);

#endif// RETRO_INPUT_H
