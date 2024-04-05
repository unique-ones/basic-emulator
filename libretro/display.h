//
// MIT License
//
// Copyright (c) 2024 Elias Engelbert Plank
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

#ifndef RETRO_DISPLAY_H
#define RETRO_DISPLAY_H

#include "types.h"

#include <GLFW/glfw3.h>

typedef struct display {
    GLFWwindow *handle;
    const char *title;
    s32 width;
    s32 height;
    bool running;
    f64 time;
} display_t;

/// Creates a new window and a corresponding OpenGL context
/// @param self The display handle
/// @param title The title of the window
/// @param width The width of the window
/// @param height The height of the window
/// @return A boolean value that indicates successful display creation
bool display_create(display_t *self, const char *title, s32 width, s32 height);

/// Destroys the window
/// @param self The display handle
void display_destroy(display_t *self);

/// Sets the argument that gets passed to every callback
/// @param self The display handle
/// @param arg The argument pointer
void display_callback_argument(display_t *self, void *arg);

/// Sets the key callback for the display
/// @param self The display handle
/// @param callback The callback
void display_key_callback(display_t *self, GLFWkeyfun callback);

/// Sets the char callback for the display
/// @param self The display handle
/// @param callback The callback
void display_char_callback(display_t *self, GLFWcharfun callback);

/// Updates the title of the window
/// @param self The display handle
/// @param title The title
void display_title(display_t *self, const char *title);

/// Swaps front and back buffer
/// @param self The display handle
/// @return The frame time
f64 display_update_frame(display_t *self);

/// Polls for incoming events
/// @param self The display handle
void display_update_input(display_t *self);

/// Checks if the window should be closed or not
/// @param self The display handle
/// @return A boolean value that indicates the the display wants to be closed
bool display_running(display_t *self);

#endif// RETRO_DISPLAY_H
