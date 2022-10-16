#include "emu.h"

emulator_t* emulator_new() {
    emulator_t* self = (emulator_t*) malloc(sizeof(emulator_t));
    self->variables = stack_new(16);
    self->subroutines = list_new();
}

void emulator_free(emulator_t* self) {
    map_free(self->variables);
    stack_free(self->subroutines);
    free(self);
}