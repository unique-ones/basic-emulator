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
 * @brief creates a new window and a corresponding OpenGL context
 * @param self display handle
 * @param title title of the window
 * @param width width of the window
 * @param height height of the window
 * @return bool
 */
bool display_create(display_t* self, const char* title, s32 width, s32 height);

/**
 * @brief set the argument that gets passed to every callback
 * @param self display handle
 * @param arg argument pointer
 */
void display_callback_argument(display_t* self, void* arg);

/**
 * @brief set the key callback for the display
 * @param self display handle
 * @param callback callback
 */
void display_key_callback(display_t* self, GLFWkeyfun callback);

/**
 * @brief set the char callback for the display
 * @param self display handle
 * @param callback callback
 */
void display_char_callback(display_t* self, GLFWcharfun callback);

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
void display_update_input(display_t* self);

/**
 * @brief checks if the window should be closed or not
 * @param self display handle
 * @return
 */
bool display_running(display_t* self);

#endif// RETRO_DISPLAY_H
