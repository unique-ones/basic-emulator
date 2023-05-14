#include <stdlib.h>

#include "stack.h"

stack_t* stack_new(u32 capacity) {
    stack_t* self = (stack_t*) malloc(sizeof(stack_t));
    self->capacity = capacity;
    self->size = 0;
    self->data = (void**) malloc(capacity * sizeof(void*));
    return self;
}

void stack_grow(stack_t* self, u32 capacity) {
    self->data = (void**) realloc(self->data, capacity * sizeof(void*));
    self->capacity = capacity;
}

void stack_free(stack_t* self) {
    free(self->data);
    self->capacity = 0;
    self->size = 0;
    free(self);
}

void stack_push(stack_t* self, void* data) {
    if (self->size == self->capacity) {
        // If we run out of memory, allocate twice the capacity
        stack_grow(self, self->capacity * 2);
    }
    self->data[self->size++] = data;
}

void* stack_pop(stack_t* self) {
    if (self->size == 0) {
        return NULL;
    }
    return self->data[--(self->size)];
}

void* stack_peek(stack_t* self) {
    if (self->size == 0) {
        return NULL;
    }
    return self->data[self->size - 1];
}
