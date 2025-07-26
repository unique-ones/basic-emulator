// Copyright (c) 2025 Elias Engelbert Plank

#ifndef RETRO_DISPLAY_H
#define RETRO_DISPLAY_H

typedef struct Display {
    GLFWwindow *handle;
    const char *title;
    s32 width;
    s32 height;
    b32 running;
    f64 time;
} Display;

/// Creates a new window and a corresponding OpenGL context
/// @param self The display handle
/// @param title The title of the window
/// @param width The width of the window
/// @param height The height of the window
/// @return A b32ean value that indicates successful display creation
static b32 display_create(Display *self, const char *title, s32 width, s32 height);

/// Destroys the window
/// @param self The display handle
static void display_destroy(Display const *self);

/// Sets the argument that gets passed to every callback
/// @param self The display handle
/// @param arg The argument pointer
static void display_callback_argument(Display const *self, void *arg);

/// Sets the key callback for the display
/// @param self The display handle
/// @param callback The callback
static void display_key_callback(Display const *self, GLFWkeyfun callback);

/// Sets the char callback for the display
/// @param self The display handle
/// @param callback The callback
static void display_char_callback(Display const *self, GLFWcharfun callback);

/// Updates the title of the window
/// @param self The display handle
/// @param title The title
static void display_title(Display *self, const char *title);

/// Swaps front and back buffer
/// @param self The display handle
/// @return The frame time
static f64 display_update_frame(Display *self);

/// Polls for incoming events
/// @param self The display handle
static void display_update_input(Display *self);

/// Checks if the window should be closed or not
/// @param self The display handle
/// @return A b32ean value that indicates that the display wants to be closed
static b32 display_running(Display const *self);

#endif// RETRO_DISPLAY_H
