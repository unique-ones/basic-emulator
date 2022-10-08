#ifndef RETRO_DISPLAY_H
#define RETRO_DISPLAY_H

#include "types.h"

#include <GLFW/glfw3.h>

typedef struct display {
    GLFWwindow* handle;
    const char* title;
    s32 width;
    s32 height;
    bool running;
    f64 time;
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
