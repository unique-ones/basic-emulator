#ifndef RETRO_DISPLAY_H
#define RETRO_DISPLAY_H

#include "input.h"
#include "types.h"

#include <GLFW/glfw3.h>

typedef struct display {
    GLFWwindow* handle;
    const char* title;
    s32 width;
    s32 height;
    bool running;
    f64 time;
    input_buffer_t* input_buffer;
} display_t;

/**
 * @brief creates a new window and a corresponding OpenGL context
 * @param self display Handle
 * @param title title of the window
 * @param width width of the window
 * @param height height of the window
 * @return bool
 */
bool display_create(display_t* self, const char* title, s32 width, s32 height);

/**
 * @brief set the input state handle for the self
 * @param self display handle
 * @param input_buffer input buffer handle
 */
void display_input(display_t* self, input_buffer_t* input_buffer);

/**
 * @brief updates the title of the window
 * @param self display handle
 * @param title title
 */
void display_title(display_t* self, const char* title);

/**
 * @brief destroys the window
 * @param self display handle
 */
void display_destroy(display_t* self);

/**
 * @brief swaps front and back buffer
 * @param self display handle
 * @return frame time
 */
f64 display_update_frame(display_t* self);

/**
 * @brief polls for incoming events
 */
void display_update_input();

/**
 * @brief checks if the window should be closed or not
 * @param self display handle
 * @return
 */
bool display_running(display_t* self);

#endif// RETRO_DISPLAY_H
