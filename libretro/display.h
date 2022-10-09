#ifndef RETRO_DISPLAY_H
#define RETRO_DISPLAY_H

#include "state.h"
#include "types.h"

#include <GLFW/glfw3.h>

typedef struct display {
    GLFWwindow* handle;
    const char* title;
    s32 width;
    s32 height;
    bool running;
    f64 time;
    input_state_t* input_state;
} display_t;

/**
 * @brief Creates a new window and a corresponding OpenGL context
 * @param display Display Handle
 * @param title Title of the window
 * @param width Width of the window
 * @param height Height of the window
 * @return bool
 */
bool display_create(display_t* display, const char* title, s32 width, s32 height);

/**
 * @brief Set the input state handle for the display
 * @param display Display handle
 * @param input_state_t Input state handle
 */
void display_input(display_t* display, input_state_t* input_state);

/**
 * @brief Updates the title of the window
 * @param display Display handle
 * @param title Title
 */
void display_title(display_t* display, const char* title);

/**
 * @brief Destroys the window
 * @param display Display handle
 */
void display_destroy(display_t* display);

/**
 * @brief Polls for events and swaps front and back buffer
 * @param display Display handle
 * @return Frame time
 */
f64 display_update(display_t* display);

/**
 * @brief Checks if the window should be closed or not
 * @param display Display handle
 * @return
 */
bool display_running(display_t* display);

#endif// RETRO_DISPLAY_H
