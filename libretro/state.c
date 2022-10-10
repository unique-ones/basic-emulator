#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "math.h"
#include "state.h"

void input_buffer_create(input_buffer_t* self, u32 capacity) {
    self->fill = 0;
    self->cursor = 0;
    self->data = (char*) malloc(capacity);
    self->capacity = capacity;
}

void input_buffer_destroy(input_buffer_t* self) {
    free(self->data);
}

bool input_buffer_emplace(input_buffer_t* self, char data) {
    if (input_buffer_is_full(self)) {
        return false;
    }
    self->data[self->cursor] = data;
    self->fill++;
    self->cursor++;
    return true;
}

static void input_buffer_reorder(input_buffer_t* buffer) {
    for (u32 i = 0; i < buffer->fill; i++) {
        char* current = buffer->data + i;
        if (*current == -1) {
            memcpy(current, current + 1, buffer->fill - i - 1);
            buffer->fill--;
            if (i < buffer->cursor) {
                buffer->cursor--;
            }
        }
    }
}

bool input_buffer_remove(input_buffer_t* self) {
    if (self->fill == 0 || self->cursor == 0) {
        return false;
    }
    self->data[self->cursor - 1] = -1;
    input_buffer_reorder(self);
    return true;
}

void input_buffer_advance_cursor(input_buffer_t* self, s32 offset) {
    self->cursor = s32_clamp(self->cursor + offset, 0, self->fill);
}

bool input_buffer_is_full(input_buffer_t* self) {
    return self->fill == self->capacity;
}

void input_state_create(input_state_t* self, u32 capacity) {
    self->mode = INPUT_MODE_INPUT;
    input_buffer_create(&self->input, capacity);
}

void input_state_destroy(input_state_t* self) {
    input_buffer_destroy(&self->input);
}