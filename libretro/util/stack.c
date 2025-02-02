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


#include <assert.h>

/// Allocates a new stack
Stack *stack_new(u32 capacity) {
    Stack *self = (Stack *) malloc(sizeof(Stack));
    self->capacity = capacity;
    self->size = 0;
    self->data = (void **) malloc(capacity * sizeof(void *));
    return self;
}

/// Grows the stack's capacity
void stack_grow(Stack *self, u32 capacity) {
    void **new_data = (void **) realloc(self->data, capacity * sizeof(void *));
    if (new_data) {
        self->data = new_data;
        self->capacity = capacity;
    } else {
        assert(false && "realloc failed");
    }
}

/// Frees the stack entries except for the data itself
void stack_free(Stack *self) {
    free(self->data);
    self->capacity = 0;
    self->size = 0;
    free(self);
}

/// Pushes the specified data onto the stack
void stack_push(Stack *self, void *data) {
    if (self->size == self->capacity) {
        // If we run out of memory, allocate twice the capacity
        stack_grow(self, self->capacity * 2);
    }
    self->data[self->size++] = data;
}

/// Pops data from the stack
void *stack_pop(Stack *self) {
    if (self->size == 0) {
        return NULL;
    }
    return self->data[--(self->size)];
}

/// Peeks the top element
void *stack_peek(Stack *self) {
    if (self->size == 0) {
        return NULL;
    }
    return self->data[self->size - 1];
}
