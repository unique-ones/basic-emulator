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
