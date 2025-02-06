// Copyright (c) 2025 Elias Engelbert Plank

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "util/math.h"

/// Creates an input buffer
void input_buffer_create(InputBuffer *self, usize capacity) {
    self->fill = 0;
    self->cursor = 0;
    self->data = (char *) malloc(capacity);
    self->capacity = capacity;
    self->submit = false;
}

/// Destroys the input buffer and frees its data
void input_buffer_destroy(InputBuffer *self) {
    free(self->data);
}

/// Inserts a character into the input buffer
bool input_buffer_emplace(InputBuffer *self, char data) {
    if (input_buffer_is_full(self)) {
        return false;
    }
    self->data[self->cursor] = data;
    self->fill++;
    self->cursor++;
    return true;
}

/// Reorders the input buffer (i.e. compactifies)
static void input_buffer_reorder(InputBuffer *buffer) {
    for (usize i = 0; i < buffer->fill; i++) {
        char *current = buffer->data + i;
        if (*current == -1) {
            memcpy(current, current + 1, (usize) (buffer->fill - i - 1));
            buffer->fill--;
            if (i < buffer->cursor) {
                buffer->cursor--;
            }
        }
    }
}

/// Removes data at the cursor, reorders buffer to be continuous in memory
bool input_buffer_remove(InputBuffer *self) {
    if (self->fill == 0 || self->cursor == 0) {
        return false;
    }
    self->data[self->cursor - 1] = -1;
    input_buffer_reorder(self);
    return true;
}

/// Advances the cursor by the specified offset
void input_buffer_advance_cursor(InputBuffer *self, s64 offset) {
    self->cursor = s32_clamp(self->cursor + offset, 0, self->fill);
}

/// Checks if the input buffer is full
bool input_buffer_is_full(InputBuffer *self) {
    return self->fill == (usize) self->capacity;
}
