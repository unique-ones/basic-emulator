// Copyright (c) 2025 Elias Engelbert Plank

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
static void emulator_pass_finish(Emulator *self) {
    text_queue_push(self->history, self->text.data, self->text.fill);
    text_cursor_clear(&self->text);
    self->state = EMULATOR_STATE_INPUT;
}

/// Runs an emulator pass
static void emulator_pass(Emulator *self) {
    // Parse user input
    TokenList *tokens = tokenize(self->text.data, self->text.fill);
    StatementResult const result = statement_compile(&self->arena, tokens->begin, tokens->end);
    token_list_free(tokens);

    F32Vector2 position = { 30.0f, 30.0f };
    if (result.type == RESULT_ERROR) {
        // show user the error
        static F32Vector3 err = { 1.0f, 0.0f, 0.0f };
        renderer_draw_text(self->renderer, &position, &err, 0.5f, "%s", result.error);
    } else {
        switch (result.statement->type) {
            case STATEMENT_RUN:
                program_execute(&self->program);
                break;
            default:
                program_tree_insert(&self->program.lines, result.statement);
                emulator_pass_finish(self);
                return;
        }
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
static f64 rnd(f64 const x) {
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
static f64 sgn(f64 const x) {
    if (x > 0.0) {
        return 1.0;
    }
    if (x < 0.0) {
        return -1.0;
    }
    return 0.0;
}

/// Adds all builtin symbols to the emulator symbol table
static void emulator_add_builtin_symbols(Emulator const *self) {
    // available math functions
    static FunctionDefinition builtin[] = { { .name = "ABS",
                                              .type = FUNCTION_DEFINITION_BUILTIN,
                                              .builtin = { .parameter_count = PARAMETER_COUNT_1, .func1 = fabs } },
                                            { "ATN", .type = FUNCTION_DEFINITION_BUILTIN,
                                              .builtin = { .parameter_count = PARAMETER_COUNT_1, .func1 = atan } },
                                            { .name = "COS",
                                              .type = FUNCTION_DEFINITION_BUILTIN,
                                              .builtin = { .parameter_count = PARAMETER_COUNT_1, .func1 = cos } },
                                            { "EXP", .type = FUNCTION_DEFINITION_BUILTIN,
                                              .builtin = { .parameter_count = PARAMETER_COUNT_1, .func1 = exp } },
                                            { .name = "INT",
                                              .type = FUNCTION_DEFINITION_BUILTIN,
                                              .builtin = { .parameter_count = PARAMETER_COUNT_1, .func1 = floor } },
                                            { "LOG", .type = FUNCTION_DEFINITION_BUILTIN,
                                              .builtin = { .parameter_count = PARAMETER_COUNT_1, .func1 = log } },
                                            { .name = "RND",
                                              .type = FUNCTION_DEFINITION_BUILTIN,
                                              .builtin = { .parameter_count = PARAMETER_COUNT_1, .func1 = rnd } },
                                            { "SGN", .type = FUNCTION_DEFINITION_BUILTIN,
                                              .builtin = { .parameter_count = PARAMETER_COUNT_1, .func1 = sgn } },
                                            { .name = "SIN",
                                              .type = FUNCTION_DEFINITION_BUILTIN,
                                              .builtin = { .parameter_count = PARAMETER_COUNT_1, .func1 = sin } },
                                            { "SQR", .type = FUNCTION_DEFINITION_BUILTIN,
                                              .builtin = { .parameter_count = PARAMETER_COUNT_1, .func1 = sqrt } },
                                            { .name = "TAN",
                                              .type = FUNCTION_DEFINITION_BUILTIN,
                                              .builtin = { .parameter_count = PARAMETER_COUNT_1, .func1 = tan } } };

    for (usize index = 0; index < STACK_ARRAY_SIZE(builtin); ++index) {
        FunctionDefinition *function = builtin + index;
        hash_map_insert(self->program.symbols, function->name, function);
    }
}

/// Creates a new emulator instance
void emulator_create(Emulator *self, Renderer *renderer) {
    self->state = EMULATOR_STATE_INPUT;
    self->mode = EMULATOR_MODE_TEXT;
    self->renderer = renderer;
    program_create(&self->program, renderer);
    emulator_add_builtin_symbols(self);

    text_cursor_create(&self->text, 128);
    self->history = text_queue_new();
    self->arena = arena_identity(ALIGNMENT8);
    self->enable_crt = true;
}

/// Destroys the emulator and frees all its associated data
void emulator_destroy(Emulator *self) {
    program_destroy(&self->program);
    text_cursor_destroy(&self->text);
    text_queue_free(self->history);
    arena_destroy(&self->arena);
}

/// Runs an emulation pass
void emulator_run(Emulator *self) {
    self->state = EMULATOR_STATE_EXECUTION;
    self->text.submit = false;
    thread_create(emulator_pass_platform, self);
}

/// Key callback handler for handling GLFW key input
void emulator_key_callback(GLFWwindow *handle, s32 const key, s32 const scancode, s32 const action, s32 const mods) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) {
        return;
    }
    Emulator *self = glfwGetWindowUserPointer(handle);
    if (self) {
        self->program.last_key = key;
        if (self->state == EMULATOR_STATE_INPUT) {
            TextCursor *text = &self->text;
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
void emulator_char_callback(GLFWwindow *handle, u32 const unicode) {
    Emulator *self = glfwGetWindowUserPointer(handle);
    if (self) {
        text_cursor_emplace(&self->text, (char) toupper((char) unicode));
    }
}
