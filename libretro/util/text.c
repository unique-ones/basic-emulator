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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "math.h"
#include "text.h"

/// Creates a text cursor with the specified capacity
void text_cursor_create(text_cursor_t* self, u32 capacity) {
    self->fill = 0;
    self->cursor = 0;
    self->data = (char*) malloc(capacity);
    self->capacity = capacity;
    self->submit = false;
}

/// Destroys the text cursor and frees its data
void text_cursor_destroy(text_cursor_t* self) {
    free(self->data);
}

/// Inserts a character into the text cursor buffer at the cursor position
bool text_cursor_emplace(text_cursor_t* self, char data) {
    if (text_cursor_is_full(self)) {
        return false;
    }
    self->data[self->cursor] = data;
    self->fill++;
    self->cursor++;
    return true;
}

/// Reorders the text cursor and compactifies the buffer
static void text_cursor_reorder(text_cursor_t* buffer) {
    for (s32 i = 0; i < buffer->fill; i++) {
        char* current = buffer->data + i;
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
bool text_cursor_remove(text_cursor_t* self) {
    if (self->fill == 0 || self->cursor == 0) {
        return false;
    }
    self->data[self->cursor - 1] = -1;
    text_cursor_reorder(self);
    return true;
}

/// Advances the cursor by the specified offset
void text_cursor_advance(text_cursor_t* self, s32 offset) {
    self->cursor = s32_clamp(self->cursor + offset, 0, self->fill);
}

/// Checks if the text cursor buffer is full
bool text_cursor_is_full(text_cursor_t* self) {
    return self->fill == (s32) self->capacity;
}

/// Clears the buffer of the text cursor
void text_cursor_clear(text_cursor_t* self) {
    memset(self->data, 0, self->capacity);
    self->cursor = 0;
    self->fill = 0;
    self->submit = false;
}

/// Creates a new text entry
text_entry_t* text_entry_new(char* data, u32 length) {
    text_entry_t* self = (text_entry_t*) malloc(sizeof(text_entry_t));
    self->prev = NULL;
    self->next = NULL;

    self->data = (char*) malloc((size_t) (length) + 1);
    self->length = length;
    memcpy(self->data, data, length);
    self->data[self->length] = '\0';
    return self;
}

/// Frees the text entry
void text_entry_free(text_entry_t* self) {
    free(self->data);
    self->prev = NULL;
    self->next = NULL;
    self->data = NULL;
    self->length = 0;
    free(self);
}

/// Creates a new text queue
text_queue_t* text_queue_new(void) {
    text_queue_t* self = (text_queue_t*) malloc(sizeof(text_queue_t));
    self->begin = NULL;
    self->end = NULL;
    self->entries = 0;
    return self;
}

/// Frees the text queue
void text_queue_free(text_queue_t* self) {
    text_queue_clear(self);
    free(self);
}

/// Clears the text queue
void text_queue_clear(text_queue_t* self) {
    text_entry_t* it = self->begin;
    text_entry_t* tmp;

    while (it != NULL) {
        tmp = it;
        it = it->next;
        text_entry_free(tmp);
    }
    self->begin = NULL;
    self->end = NULL;
    self->entries = 0;
}

/// Pushes data to the text queue
void text_queue_push(text_queue_t* self, char* data, u32 length) {
    text_entry_t* entry = text_entry_new(data, length);
    if (self->begin == NULL) {
        self->begin = entry;
        entry->prev = NULL;
        self->entries++;
        return;
    }

    text_entry_t* tmp = self->begin;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }

    tmp->next = entry;
    entry->prev = tmp;
    self->end = entry;
    self->entries++;
}

/// Pushes format strings to the text queue
void text_queue_push_format(text_queue_t* self, const char* fmt, ...) {
    char buffer[1024];
    va_list list;
    va_start(list, fmt);
    u32 length = (u32) vsnprintf(buffer, sizeof buffer, fmt, list);
    va_end(list);
    text_queue_push(self, buffer, length);
}
