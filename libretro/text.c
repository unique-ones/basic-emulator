#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "math.h"
#include "text.h"

void text_queue_create(text_queue_t* self, u32 capacity) {
    self->fill = 0;
    self->cursor = 0;
    self->data = (char*) malloc(capacity);
    self->capacity = capacity;
    self->submit = false;
}

void text_queue_destroy(text_queue_t* self) {
    free(self->data);
}

bool text_queue_emplace(text_queue_t* self, char data) {
    if (text_queue_is_full(self)) {
        return false;
    }
    self->data[self->cursor] = data;
    self->fill++;
    self->cursor++;
    return true;
}

static void text_queue_reorder(text_queue_t* buffer) {
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

bool text_queue_remove(text_queue_t* self) {
    if (self->fill == 0 || self->cursor == 0) {
        return false;
    }
    self->data[self->cursor - 1] = -1;
    text_queue_reorder(self);
    return true;
}

void text_queue_advance_cursor(text_queue_t* self, s32 offset) {
    self->cursor = s32_clamp(self->cursor + offset, 0, self->fill);
}

bool text_queue_is_full(text_queue_t* self) {
    return self->fill == self->capacity;
}
void text_queue_clear(text_queue_t* self) {
    memset(self->data, 0, self->capacity);
    self->cursor = 0;
    self->fill = 0;
    self->submit = false;
}


text_entry_t* text_entry_new(char* data, u32 length) {
    text_entry_t* self = (text_entry_t*) malloc(sizeof(text_entry_t));
    self->prev = NULL;
    self->next = NULL;

    self->data = (char*) malloc(length + 1);
    self->length = length;
    memcpy(self->data, data, length);
    self->data[self->length] = '\0';
    return self;
}

void text_entry_free(text_entry_t* self) {
    free(self->data);
    self->prev = NULL;
    self->next = NULL;
    self->data = NULL;
    self->length = 0;
    free(self);
}

text_output_queue_t* text_output_queue_new(void) {
    text_output_queue_t* self = (text_output_queue_t*) malloc(sizeof(text_output_queue_t));
    self->begin = NULL;
    self->end = NULL;
    self->entries = 0;
    return self;
}

void text_output_queue_free(text_output_queue_t* self) {
    text_output_queue_clear(self);
    free(self);
}

void text_output_queue_clear(text_output_queue_t* self) {
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

void text_output_queue_push(text_output_queue_t* self, char* data, u32 length) {
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