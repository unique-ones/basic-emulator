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
