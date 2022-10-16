#ifndef RETRO_EMU_H
#define RETRO_EMU_H

#include <pthread.h>

#include "input.h"
#include "map.h"
#include "stack.h"

typedef struct emulator {
    pthread_t thread_id;
    map_t* variables;
    stack_t* subroutines;
} emulator_t;

/**
 * @brief allocates a new emulator instance
 * @return reference to emulator instance
 */
emulator_t* emulator_new();

/**
 * @brief deallocates the emulator and frees all its associated data
 * @param self reference to emulator instance
 */
void emulator_free(emulator_t* self);

/**
 * @brief executes the specified input
 * @param self emulator instance
 * @param input basic code
 */
bool emulator_execute(emulator_t* self, const char* input);

#endif// RETRO_EMU_H