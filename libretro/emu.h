#ifndef RETRO_EMU_H
#define RETRO_EMU_H

#include <GLFW/glfw3.h>

#include "map.h"
#include "renderer.h"
#include "stack.h"
#include "text.h"

#define EMULATOR_MEMORY_SIZE (64 * 1024)

typedef enum emulator_state { EMULATOR_STATE_INPUT = 0, EMULATOR_STATE_EXECUTION = 1 } emulator_state_t;
typedef enum emulator_mode { EMULATOR_MODE_TEXT = 0, EMULATOR_MODE_GRAPHICS = 1 } emulator_mode_t;

typedef struct emulator {
    emulator_state_t state;
    emulator_mode_t mode;
    renderer_t* renderer;

    map_t* symbols;
    map_t* lines;
    stack_t* stack;

    text_cursor_t text;
    text_queue_t* history;

    s32 last_key;
    u8* memory;
} emulator_t;

/**
 * @brief allocates a new emulator instance
 * @param self reference to emulator instance
 */
void emulator_create(emulator_t* self, renderer_t* renderer);

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

/**
 * @brief key callback handler for handling key input
 * @param handle glfw window handle
 * @param key key that is currently pressed
 * @param scancode scancode of the key
 * @param action action that indicates if the key is pressed or released
 * @param mods don't know about this one
 */
void emulator_key_callback(GLFWwindow* handle, s32 key, s32 scancode, s32 action, s32 mods);

/**
 * @brief char callback handler for handling key input
 * @param handle glfw window handle
 * @param unicode character in unicode
 */
void emulator_char_callback(GLFWwindow* handle, u32 unicode);


#endif// RETRO_EMU_H
