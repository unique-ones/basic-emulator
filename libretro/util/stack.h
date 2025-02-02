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

#ifndef RETRO_UTIL_STACK_H
#define RETRO_UTIL_STACK_H

#include "types.h"

typedef struct Stack {
    void **data;
    u32 size;
    u32 capacity;
} Stack;

/// Allocates a new stack
/// @param capacity The initial capacity
/// @return A new stack instance
Stack *stack_new(u32 capacity);

/// Grows the stack's capacity
/// @param self The stack handle
/// @param capacity The new capacity
void stack_grow(Stack *self, u32 capacity);

/// Frees the stack entries except for the data itself
/// @param self The stack handle
void stack_free(Stack *self);

/// Pushes the specified data onto the stack
/// @param self The stack handle
/// @param data The data
void stack_push(Stack *self, void *data);

/// Pops data from the stack
/// @param self The stack handle
/// @return The top-most data
void *stack_pop(Stack *self);

/// Peeks the top element
/// @param self The stack handle
/// @return The top-most data
void *stack_peek(Stack *self);

#endif// RETRO_UTIL_STACK_H
