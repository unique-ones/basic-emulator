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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "util/math.h"

/// Creates an input buffer
void input_buffer_create(input_buffer_t *self, u32 capacity) {
    self->fill = 0;
    self->cursor = 0;
    self->data = (char *) malloc(capacity);
    self->capacity = capacity;
    self->submit = false;
}

/// Destroys the input buffer and frees its data
void input_buffer_destroy(input_buffer_t *self) {
    free(self->data);
}

/// Inserts a character into the input buffer
bool input_buffer_emplace(input_buffer_t *self, char data) {
    if (input_buffer_is_full(self)) {
        return false;
    }
    self->data[self->cursor] = data;
    self->fill++;
    self->cursor++;
    return true;
}

/// Reorders the input buffer (i.e. compactifies)
static void input_buffer_reorder(input_buffer_t *buffer) {
    for (s32 i = 0; i < buffer->fill; i++) {
        char *current = buffer->data + i;
        if (*current == -1) {
            memcpy(current, current + 1, (size_t) (buffer->fill - i - 1));
            buffer->fill--;
            if (i < buffer->cursor) {
                buffer->cursor--;
            }
        }
    }
}

/// Removes data at the cursor, reorders buffer to be continuous in memory
bool input_buffer_remove(input_buffer_t *self) {
    if (self->fill == 0 || self->cursor == 0) {
        return false;
    }
    self->data[self->cursor - 1] = -1;
    input_buffer_reorder(self);
    return true;
}

/// Advances the cursor by the specified offset
void input_buffer_advance_cursor(input_buffer_t *self, s32 offset) {
    self->cursor = s32_clamp(self->cursor + offset, 0, self->fill);
}

/// Checks if the input buffer is full
bool input_buffer_is_full(input_buffer_t *self) {
    return self->fill == (s32) self->capacity;
}
