#include <stdlib.h>

#include "emu.h"

emulator_t* emulator_new() {
    emulator_t* self = (emulator_t*) malloc(sizeof(emulator_t));
    self->state = EMULATOR_STATE_INPUT;
    self->variables = map_new();
    self->subroutines = stack_new(255);
    return self;
}

void emulator_free(emulator_t* self) {
    map_free(self->variables);
    stack_free(self->subroutines);
    free(self);
}