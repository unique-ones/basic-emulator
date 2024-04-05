//
// MIT License
//
// Copyright (c) 2023 Elias Engelbert Plank
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "arch/thread.h"
#include "arch/time.h"
#include "emu.h"
#include "expr.h"
#include "lexer.h"
#include "stmt.h"
#include "util/random.h"

#include "GLFW/glfw3.h"

/// Finalizes an emulator pass by destroying associated data
static void emulator_pass_finish(emulator_t *self) {
    text_queue_push(self->history, self->text.data, (u32) self->text.fill);
    text_queue_clear(self->program.output);
    text_cursor_clear(&self->text);
    self->state = EMULATOR_STATE_INPUT;
}

/// Runs an emulator pass
static void emulator_pass(emulator_t *self) {
    // Parse user input
    token_list_t *tokens = tokenize(self->text.data, (u32) self->text.fill);
    statement_t *statement = statement_compile(&self->arena, tokens->begin, tokens->end);
    token_list_free(tokens);

    f32vec2_t position = { 30.0f, 30.0f };
    if (statement == NULL) {
        // show user the error
        static f32vec3_t err = { 1.0f, 0.0f, 0.0f };
        renderer_draw_text(self->renderer, &position, &err, 0.5f, "ERROR: INVALID STATEMENT!\n");
    } else {
        // if we encounter the RUN statement, we must iterate over all the lines.
        // currently, this is not implemented. therefore, we only execute one
        // statement at a time.
        if (statement->type == STATEMENT_RUN) {
            program_execute(&self->program);
        } else {
            program_tree_insert(&self->program.lines, statement);
            emulator_pass_finish(self);
            return;
        }
    }

    text_queue_t *output = self->program.output;
    for (text_entry_t *it = output->begin; it != NULL; it = it->next) {
        static f32vec3_t msg = { 1.0f, 0.55f, 0.0f };
        renderer_draw_text(self->renderer, &position, &msg, 0.5f, "%.*s", it->length, it->data);
    }

    while (self->program.last_key != GLFW_KEY_ESCAPE) {
        // here we waste time
        time_sleep(10);
    }

    emulator_pass_finish(self);
}

#ifdef LIBRETRO_PLATFORM_WIN32

/// Win32 specific emulator pass (thread param)
static unsigned long emulator_pass_platform(void *self) {
    emulator_pass(self);
    return 0;
}
#else

/// Unix specific emulator pass (thread param)
static void *emulator_pass_platform(void *self) {
    emulator_pass(self);
    return NULL;
}
#endif

/// Random number generation as of applesoft basic
static f64 rnd(f64 x) {
    static f64 previous = 0.5;
    if (x == 0.0) {
        return previous;
    }
    if (x < 0.0) {
        random_seed(time(NULL));
    }

    previous = (f64) (random_u64()) / (f64) UINT64_MAX;
    return previous;
}

/// Retrieves the sign of the specified number
static f64 sgn(f64 x) {
    if (x > 0.0) {
        return 1.0;
    }
    if (x < 0.0) {
        return -1.0;
    }
    return 0.0;
}

/// Adds all builtin symbols to the emulator symbol table
static void emulator_add_builtin_symbols(emulator_t *self) {
    // available math functions
    static function_definition_t builtin[] = { { "ABS", 1, { .func1 = fabs } },  { "ATN", 1, { .func1 = atan } },
                                               { "COS", 1, { .func1 = cos } },   { "EXP", 1, { .func1 = exp } },
                                               { "INT", 1, { .func1 = floor } }, { "LOG", 1, { .func1 = log } },
                                               { "RND", 1, { .func1 = rnd } },   { "SGN", 1, { .func1 = sgn } },
                                               { "SIN", 1, { .func1 = sin } },   { "SQR", 1, { .func1 = sqrt } },
                                               { "TAN", 1, { .func1 = tan } } };

    for (u32 index = 0; index < STACK_ARRAY_SIZE(builtin); ++index) {
        function_definition_t *function = builtin + index;
        map_insert(self->program.symbols, function->name, function);
    }
}

/// Creates a new emulator instance
void emulator_create(emulator_t *self, renderer_t *renderer) {
    self->state = EMULATOR_STATE_INPUT;
    self->mode = EMULATOR_MODE_TEXT;
    self->renderer = renderer;
    program_create(&self->program, EMULATOR_MEMORY_SIZE);
    emulator_add_builtin_symbols(self);

    text_cursor_create(&self->text, 128);
    self->history = text_queue_new();
    self->arena = arena_identity(ALIGNMENT8);
    self->enable_crt = true;
}

/// Destroys the emulator and frees all its associated data
void emulator_destroy(emulator_t *self) {
    program_destroy(&self->program);
    text_cursor_destroy(&self->text);
    text_queue_free(self->history);
    arena_destroy(&self->arena);
}

/// Runs an emulation pass
void emulator_run(emulator_t *self) {
    self->state = EMULATOR_STATE_EXECUTION;
    self->text.submit = false;
    thread_create(emulator_pass_platform, self);
}

/// Key callback handler for handling GLFW key input
void emulator_key_callback(GLFWwindow *handle, s32 key, s32 scancode, s32 action, s32 mods) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) {
        return;
    }
    emulator_t *self = glfwGetWindowUserPointer(handle);
    if (self) {
        self->program.last_key = key;
        if (self->state == EMULATOR_STATE_INPUT) {
            text_cursor_t *text = &self->text;
            switch (key) {
                case GLFW_KEY_LEFT:
                    text_cursor_advance(text, -1);
                    break;
                case GLFW_KEY_RIGHT:
                    text_cursor_advance(text, 1);
                    break;
                case GLFW_KEY_BACKSPACE:
                    text_cursor_remove(text);
                    break;
                case GLFW_KEY_TAB:
                    text_cursor_emplace(text, '\t');
                    break;
                case GLFW_KEY_ENTER:
                    text_cursor_emplace(text, '\n');
                    text->submit = true;
                    break;
                case GLFW_KEY_F2:
                    self->enable_crt = !self->enable_crt;
                    break;
                default:
                    break;
            }
        }
    }
}

/// Char callback handler for handling GLFW char input
void emulator_char_callback(GLFWwindow *handle, u32 unicode) {
    emulator_t *self = glfwGetWindowUserPointer(handle);
    if (self) {
        text_cursor_emplace(&self->text, (char) toupper((char) unicode));
    }
}
