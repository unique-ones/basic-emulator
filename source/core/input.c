// Copyright (c) 2025 Elias Engelbert Plank

/// Creates an input buffer
static void input_buffer_create(InputBuffer *self, ssize const capacity) {
    self->fill = 0;
    self->cursor = 0;
    self->data = (char *) malloc(capacity);
    self->capacity = capacity;
    self->submit = false;
}

/// Destroys the input buffer and frees its data
static void input_buffer_destroy(InputBuffer const *self) {
    free(self->data);
}

/// Inserts a character into the input buffer
static b32 input_buffer_emplace(InputBuffer *self, char const data) {
    if (input_buffer_is_full(self)) {
        return false;
    }
    self->data[self->cursor] = data;
    self->fill++;
    self->cursor++;
    return true;
}

/// Reorders the input buffer (i.e. compactions)
static void input_buffer_reorder(InputBuffer *buffer) {
    for (usize i = 0; i < buffer->fill; i++) {
        char *current = buffer->data + i;
        if (*current == -1) {
            memcpy(current, current + 1, (buffer->fill - i - 1));
            buffer->fill--;
            if (i < buffer->cursor) {
                buffer->cursor--;
            }
        }
    }
}

/// Removes data at the cursor, reorders buffer to be continuous in memory
static b32 input_buffer_remove(InputBuffer *self) {
    if (self->fill == 0 || self->cursor == 0) {
        return false;
    }
    self->data[self->cursor - 1] = -1;
    input_buffer_reorder(self);
    return true;
}

/// Advances the cursor by the specified offset
static void input_buffer_advance_cursor(InputBuffer *self, ssize const offset) {
    self->cursor = s64_clamp(self->cursor + offset, 0, self->fill);
}

/// Checks if the input buffer is full
static b32 input_buffer_is_full(InputBuffer const *self) {
    return self->fill == self->capacity;
}
