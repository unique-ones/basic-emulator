// Copyright (c) 2025 Elias Engelbert Plank

#ifndef RETRO_UTIL_STACK_H
#define RETRO_UTIL_STACK_H

typedef struct Stack {
    void **data;
    usize size;
    usize capacity;
} Stack;

/// Allocates a new stack
/// @param capacity The initial capacity
/// @return A new stack instance
Stack *stack_new(usize capacity);

/// Grows the stack's capacity
/// @param self The stack handle
/// @param capacity The new capacity
void stack_grow(Stack *self, usize capacity);

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
void *stack_peek(Stack const *self);

#endif// RETRO_UTIL_STACK_H
