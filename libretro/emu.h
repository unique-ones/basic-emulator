#ifndef RETRO_EMU_H
#define RETRO_EMU_H

#include <GLFW/glfw3.h>

#include "map.h"
#include "renderer.h"
#include "stack.h"
#include "text.h"

typedef enum token_type {
    TOKEN_CHARACTER,
    TOKEN_DIGIT
} token_type_t;

typedef struct token {
    token_type_t type;
    char* lexeme;
    u32 length;
} token_t;

typedef struct token_list {
    token_type_t* begin;
    token_type_t* end;
    u32 tokens;
} token_list_t;

#define EMULATOR_MEMORY_SIZE (64 * 1024)

typedef enum emulator_state { EMULATOR_STATE_INPUT = 0, EMULATOR_STATE_EXECUTION = 1 } emulator_state_t;
typedef enum emulator_mode { EMULATOR_MODE_TEXT = 0, EMULATOR_MODE_GRAPHICS = 1 } emulator_mode_t;

typedef struct emulator {
    emulator_state_t state;
    emulator_mode_t mode;
    renderer_t* renderer;

    map_t* lines;
    map_t* variables;
    stack_t* subroutines;

    u8* memory;
    text_queue_t text;
    s32 last_key;
} emulator_t;

/**
 * @brief allocates a new emulator instance
 * @param self reference to emulator instance
 */
void emulator_create(emulator_t *self, renderer_t* renderer);

/**
 * @brief deallocates the emulator and frees all its associated data
 * @param self reference to emulator instance
 */
void emulator_destroy(emulator_t* self);

/**
 * @brief run an emulation pass
 * @param self emulator instance
 */
void emulator_run(emulator_t* self);

void emulator_key_callback(GLFWwindow* handle, s32 key, s32 scancode, s32 action, s32 mods);
void emulator_char_callback(GLFWwindow* handle, u32 unicode);

#endif// RETRO_EMU_H