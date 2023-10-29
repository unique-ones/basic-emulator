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

static void emulator_pass_finish(emulator_t* self) {
    text_queue_push(self->history, self->text.data, (u32) self->text.fill);
    text_cursor_clear(&self->text);
    self->state = EMULATOR_STATE_INPUT;
}

static void emulator_pass(emulator_t* self) {
    // step 1: tokenize
    token_list_t* tokens = tokenize(self->text.data, (u32) self->text.fill);

    // step 2: parse into statement
    statement_t* statement = statement_compile(tokens->begin, tokens->end);
    if (statement) {
        statement_free(statement);
    }

    // free tokens
    token_list_free(tokens);

    // step 3: execute
    // the text output queue servers as our
    // print buffer, which will be sent to the renderer
    // after every command execution
    text_queue_t* output = text_queue_new();
    text_queue_push_format(output, "foobar");

    // drawing of the output queue
    f32vec2_t position_iterator = { 0.0f, 0.0f };
    for (text_entry_t* it = output->begin; it; it = it->next) {
        renderer_draw_text(self->renderer, &position_iterator, &(f32vec3_t){ 0.0f, 0.0f, 1.0f }, 0.5f, it->data);
    }
    while (self->program.last_key != GLFW_KEY_ESCAPE) {
        // here we waste time
        time_sleep(10);
    }
    text_queue_free(output);

    // cleanup state
    emulator_pass_finish(self);
}

#ifdef LIBRETRO_PLATFORM_WIN32
static unsigned long emulator_pass_platform(void* self) {
    emulator_pass(self);
    return 0;
}
#else
static void* emulator_pass_platform(void* self) {
    emulator_pass(self);
    return NULL;
}
#endif

static f64 emulator_abs(f64 x) {
    return fabs(x);
}

static f64 emulator_floor(f64 x) {
    return floor(x);
}

static f64 emulator_sin(f64 x) {
    return sin(x);
}

static f64 emulator_atan(f64 x) {
    return atan(x);
}

static f64 emulator_log(f64 x) {
    return log(x);
}

static f64 emulator_sqrt(f64 x) {
    return sqrt(x);
}

static f64 emulator_cos(f64 x) {
    return cos(x);
}

static f64 emulator_tan(f64 x) {
    return tan(x);
}

static f64 emulator_exp(f64 x) {
    return exp(x);
}

static f64 emulator_rnd(f64 x) {
    static f64 previous = 0.5;
    if (x == 0.0) {
        return previous;
    }
    if (x < 0.0) {
        random_seed(time(NULL));
    }

    previous = (f64) (random_u64()) / (f64) RAND_MAX;
    return previous;
}

static f64 emulator_sgn(f64 x) {
    if (x > 0.0) {
        return 1.0;
    }
    if (x < 0.0) {
        return -1.0;
    }
    return 0.0;
}

static void emulator_add_builtin_symbols(emulator_t* self) {
    // available math functions
    static function_definition_t builtin[] = {
        { "ABS", 1, { .func1 = emulator_abs } },   { "ATN", 1, { .func1 = emulator_atan } },
        { "COS", 1, { .func1 = emulator_cos } },   { "EXP", 1, { .func1 = emulator_exp } },
        { "INT", 1, { .func1 = emulator_floor } }, { "LOG", 1, { .func1 = emulator_log } },
        { "RND", 1, { .func1 = emulator_rnd } },   { "SGN", 1, { .func1 = emulator_sgn } },
        { "SIN", 1, { .func1 = emulator_sin } },   { "SQR", 1, { .func1 = emulator_sqrt } },
        { "TAN", 1, { .func1 = emulator_tan } }
    };

    for (u32 index = 0; index < STACK_ARRAY_SIZE(builtin); ++index) {
        function_definition_t* function = builtin + index;
        map_insert(self->program.symbols, function->name, function);
    }
}

void emulator_create(emulator_t* self, renderer_t* renderer) {
    self->state = EMULATOR_STATE_INPUT;
    self->mode = EMULATOR_MODE_TEXT;
    self->renderer = renderer;
    program_create(&self->program, EMULATOR_MEMORY_SIZE);
    emulator_add_builtin_symbols(self);

    text_cursor_create(&self->text, 128);
    self->history = text_queue_new();
}

void emulator_destroy(emulator_t* self) {
    program_destroy(&self->program);
    text_cursor_destroy(&self->text);
    text_queue_free(self->history);
}

void emulator_run(emulator_t* self) {
    self->state = EMULATOR_STATE_EXECUTION;
    self->text.submit = false;
    thread_create(emulator_pass_platform, self);
}

void emulator_key_callback(GLFWwindow* handle, s32 key, s32 scancode, s32 action, s32 mods) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) {
        return;
    }
    emulator_t* self = glfwGetWindowUserPointer(handle);
    if (self) {
        self->program.last_key = key;
        if (self->state == EMULATOR_STATE_INPUT) {
            text_cursor_t* text = &self->text;
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
                default:
                    break;
            }
        }
    }
}

void emulator_char_callback(GLFWwindow* handle, u32 unicode) {
    emulator_t* self = glfwGetWindowUserPointer(handle);
    if (self) {
        text_cursor_emplace(&self->text, (char) toupper((char) unicode));
    }
}
