#ifndef RETRO_STATE_H
#define RETRO_STATE_H

#include "types.h"

#define STATE_INPUT_BUFFER_SIZE 128

typedef struct input_buffer {
    char data[STATE_INPUT_BUFFER_SIZE];
    s32 fill;
    s32 cursor;
} input_buffer_t;

/**
 * @brief Creates an input buffer
 * @param buffer Buffer handle
 */
void input_buffer_create(input_buffer_t* buffer);

/**
 * @brief Inserts a character into the input buffer
 * @param buffer Buffer handle
 * @param data Data
 * @return bool
 */
bool input_buffer_emplace(input_buffer_t* buffer, char data);

/**
 * @brief Removes data at the cursor, reorders buffer to be continuous in memory
 * @param buffer Buffer handle
 * @return bool
 */
bool input_buffer_remove(input_buffer_t* buffer);

/**
 * @brief Advances the cursor by the specified offset
 * @param buffer Input buffer handle
 * @param offset Offset
 */
void input_buffer_advance_cursor(input_buffer_t* buffer, s32 offset);

/**
 * @brief Checks if the input buffer is full
 * @return bool
 */
bool input_buffer_is_full(input_buffer_t* buffer);

typedef enum input_mode { INPUT_MODE_INPUT = 0, INPUT_MODE_EXECUTE = 1 } input_mode_t;

typedef struct input_state {
    input_buffer_t input;
    input_mode_t mode;
} input_state_t;

/**
 * @brief Creates an input state
 * @param state State handle
 */
void input_state_create(input_state_t* state);

#endif// RETRO_STATE_H
