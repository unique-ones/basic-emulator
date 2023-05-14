#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "arch/thread.h"
#include "arch/time.h"
#include "emu.h"

static void* emulator_pass(void* emu) {
    emulator_t* self = (emulator_t*) emu;

    // step 1: tokenize
    // step 2: parse into command queue
    // step 3: execute
    text_output_queue_t* output = text_output_queue_new();

    // for now, we will just push the input buffer because we want to see if this system works
    text_output_queue_push(output, self->text.data, self->text.fill);

    while (self->last_key != GLFW_KEY_ESCAPE) {
        // here we need to check the command queue if there are any commands left


        // drawing of the output queue
        f32vec2_t position_iterator = { 0.0f, 0.0f };
        for (text_entry_t* it = output->begin; it; it = it->next) {
            renderer_draw_text(self->renderer, &position_iterator, &(f32vec3_t){ 0.0f, 0.0f, 1.0f }, 0.5f, it->data);
        }
    }
    text_output_queue_free(output);

    // switch back to input state
    self->state = EMULATOR_STATE_INPUT;
    return NULL;
}

void emulator_create(emulator_t* self, renderer_t* renderer) {
    self->state = EMULATOR_STATE_INPUT;
    self->mode = EMULATOR_MODE_TEXT;
    self->renderer = renderer;

    self->lines = map_new();
    self->variables = map_new();
    self->subroutines = stack_new(255);

    self->memory = (u8*) malloc(EMULATOR_MEMORY_SIZE);
    text_queue_create(&self->text, 128);
}

void emulator_destroy(emulator_t* self) {
    map_free(self->lines);
    self->lines = NULL;

    map_free(self->variables);
    self->variables = NULL;

    stack_free(self->subroutines);
    self->subroutines = NULL;

    free(self->memory);
    self->memory = NULL;

    text_queue_destroy(&self->text);
}

void emulator_run(emulator_t* self) {
    self->state = EMULATOR_STATE_EXECUTION;
    self->text.submit = false;
    thread_create(emulator_pass, self);
}

void emulator_key_callback(GLFWwindow* handle, s32 key, s32 scancode, s32 action, s32 mods) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) {
        return;
    }
    emulator_t* self = glfwGetWindowUserPointer(handle);
    if (self) {
        self->last_key = key;
        text_queue_t* text = &self->text;
        switch (key) {
            case GLFW_KEY_LEFT:
                text_queue_advance_cursor(text, -1);
                break;
            case GLFW_KEY_RIGHT:
                text_queue_advance_cursor(text, 1);
                break;
            case GLFW_KEY_BACKSPACE:
                text_queue_remove(text);
                break;
            case GLFW_KEY_TAB:
                text_queue_emplace(text, '\t');
                break;
            case GLFW_KEY_ENTER:
                text_queue_emplace(text, '\n');
                text->submit = true;
                break;
            default:
                break;
        }
    }
}

void emulator_char_callback(GLFWwindow* handle, u32 unicode) {
    emulator_t* self = glfwGetWindowUserPointer(handle);
    if (self) {
        text_queue_emplace(&self->text, (char) toupper((char) unicode));
    }
}