#include "state.h"
#include <stdio.h>
#include <string.h>
#include "math.h"

void input_buffer_create(input_buffer_t* buffer) {
    buffer->fill = 0;
    buffer->cursor = 0;
    memset(buffer->data, 0, sizeof buffer->data);
}

bool input_buffer_emplace(input_buffer_t* buffer, char data) {
    if (input_buffer_is_full(buffer)) {
        return false;
    }
    buffer->data[buffer->cursor] = data;
    buffer->fill++;
    buffer->cursor++;
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

bool input_buffer_remove(input_buffer_t* buffer) {
    if (buffer->fill == 0 || buffer->cursor == 0) {
        return false;
    }
    buffer->data[buffer->cursor - 1] = -1;
    input_buffer_reorder(buffer);
    return true;
}

void input_buffer_advance_cursor(input_buffer_t* buffer, s32 offset) {
    buffer->cursor = s32_clamp(buffer->cursor + offset, 0, buffer->fill);
}

bool input_buffer_is_full(input_buffer_t* buffer) {
    return buffer->fill == STATE_INPUT_BUFFER_SIZE;
}

void input_state_create(input_state_t* state) {
    state->mode = INPUT_MODE_INPUT;
    input_buffer_create(&state->input);
}
