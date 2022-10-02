#ifndef RETRO_DISPLAY_H
#define RETRO_DISPLAY_H

#include "types.h"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

typedef struct display {
    GLFWwindow* handle;
    const char* title;
    u32 width;
    u32 height;
    bool running;
} display_t;

bool display_create(display_t* display);
void display_destroy(display_t* display);
void display_update(display_t* display);
bool display_running(display_t* display);

#endif// RETRO_DISPLAY_H
