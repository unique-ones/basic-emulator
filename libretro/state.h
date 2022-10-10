#ifndef RETRO_STATE_H
#define RETRO_STATE_H

#include "types.h"

typedef struct input_buffer {
    char* data;
    s32 fill;
    s32 cursor;
    u32 capacity;
} input_buffer_t;

/**
 * @brief creates an input buffer
 * @param self buffer handle
 * @param capacity capacity of the input buffer
 */
void input_buffer_create(input_buffer_t* self, u32 capacity);

/**
 * @brief destroys the input buffer and frees its data
 * @param self buffer handle
 */
void input_buffer_destroy(input_buffer_t* self);

/**
 * @brief inserts a character into the input buffer
 * @param self buffer handle
 * @param data data
 * @return bool
 */
bool input_buffer_emplace(input_buffer_t* self, char data);

/**
 * @brief removes data at the cursor, reorders buffer to be continuous in memory
 * @param self buffer handle
 * @return bool
 */
bool input_buffer_remove(input_buffer_t* self);

/**
 * @brief advances the cursor by the specified offset
 * @param self input buffer handle
 * @param offset offset
 */
void input_buffer_advance_cursor(input_buffer_t* self, s32 offset);

/**
 * @brief checks if the input buffer is full
 * @return bool
 */
bool input_buffer_is_full(input_buffer_t* self);

typedef enum input_mode { INPUT_MODE_INPUT = 0, INPUT_MODE_EXECUTE = 1 } input_mode_t;

typedef struct input_state {
    input_buffer_t input;
    input_mode_t mode;
} input_state_t;

/**
 * @brief creates an input state
 * @param self state handle
 * @param capacity capacity of the input buffer
 */
void input_state_create(input_state_t* self, u32 capacity);

/**
 * @brief destroys the input state and frees the input buffer
 * @param self reference to input state
 */
void input_state_destroy(input_state_t* self);

/**
 * Here we will eventually have a program state with the following components:
 * - virtual memory buffer
 * - variable map
 * - subroutine stack
 */

#endif// RETRO_STATE_H
