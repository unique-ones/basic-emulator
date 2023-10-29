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

#ifndef RETRO_STATE_H
#define RETRO_STATE_H

#include "types.h"

typedef struct input_buffer {
    char* data;
    s32 fill;
    s32 cursor;
    u32 capacity;
    bool submit;
} input_buffer_t;

/**
 * @brief creates an input buffer
 * @param self buffer handle
 * @param capacity capacity of the input buffer
 */
void input_buffer_create(input_buffer_t* self, u32 capacity);

/**
 * @brief destroys the input buffer and frees its data
 * @param self buffer handle
 */
void input_buffer_destroy(input_buffer_t* self);

/**
 * @brief inserts a character into the input buffer
 * @param self buffer handle
 * @param data data
 * @return bool
 */
bool input_buffer_emplace(input_buffer_t* self, char data);

/**
 * @brief removes data at the cursor, reorders buffer to be continuous in memory
 * @param self buffer handle
 * @return bool
 */
bool input_buffer_remove(input_buffer_t* self);

/**
 * @brief advances the cursor by the specified offset
 * @param self input buffer handle
 * @param offset offset
 */
void input_buffer_advance_cursor(input_buffer_t* self, s32 offset);

/**
 * @brief checks if the input buffer is full
 * @return bool
 */
bool input_buffer_is_full(input_buffer_t* self);

#endif// RETRO_STATE_H
