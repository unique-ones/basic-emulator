#ifndef RETRO_EMU_H
#define RETRO_EMU_H

#include "stack.h"

typedef struct program_state {
    stack_t* subroutines;
} program_state_t;

#endif// RETRO_EMU_H