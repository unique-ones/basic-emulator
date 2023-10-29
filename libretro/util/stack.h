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

#ifndef RETRO_COLLECTIONS_STACK_H
#define RETRO_COLLECTIONS_STACK_H

#include "types.h"

typedef struct stack {
    void** data;
    u32 size;
    u32 capacity;
} stack_t;

/**
 * @brief allocates a new stack
 * @param capacity initial capacity
 * @return allocated stack instance
 */
stack_t* stack_new(u32 capacity);

/**
 * @brief resizes the stack's capacity
 * @param self reference to stack instance
 * @param capacity new capacity
 */
void stack_grow(stack_t* self, u32 capacity);

/**
 * @brief frees the stack entries except for the data itself
 * @param self reference to stack instance
 */
void stack_free(stack_t* self);

/**
 * @brief pushes the specified data onto the stack
 * @param self reference to stack instance
 * @param data reference to the data
 */
void stack_push(stack_t* self, void* data);

/**
 * @brief pops data from the stack
 * @param self reference to stack instance
 * @return top-most data
 */
void* stack_pop(stack_t* self);

/**
 * @brief peek the top element
 * @param self reference to stack instance
 * @return top-most data
 */
void* stack_peek(stack_t* self);

#endif// RETRO_COLLECTIONS_STACK_H
