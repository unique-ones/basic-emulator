#ifndef RETRO_EMU_H
#define RETRO_EMU_H

#include "input.h"
#include "map.h"
#include "stack.h"

typedef enum emulator_state { EMULATOR_STATE_INPUT = 0, EMULATOR_STATE_EXECUTION = 1 } emulator_state_t;

typedef struct emulator {
    emulator_state_t state;
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

#endif// RETRO_EMU_H