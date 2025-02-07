// Copyright (c) 2025 Elias Engelbert Plank

#include <glad/glad.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "display.h"

/// Callback for framebuffer resize events
static void display_framebuffer_callback(GLFWwindow *handle, s32 const width, s32 const height) {
    glViewport(0, 0, width, height);
}

/// Converts OpenGL severities to human-readable strings
static const char *severity_string(u32 const severity) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH: {
            return "HIGH_SEVERITY";
        }
        case GL_DEBUG_SEVERITY_MEDIUM: {
            return "MEDIUM_SEVERITY";
        }
        case GL_DEBUG_SEVERITY_LOW: {
            return "LOW_SEVERITY";
        }
        case GL_DEBUG_SEVERITY_NOTIFICATION: {
            return "NOTIFICATION_SEVERITY";
        }
        default: {
            return "UNKNOWN_SEVERITY";
        }
    }
}

/// OpenGL error callback function
static void display_error_callback(u32 source,
                                   u32 const type,
                                   u32 const id,
                                   u32 const severity,
                                   s32 const length,
                                   char const *message,
                                   void const *user) {
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
        return;
    }

    if (type == GL_DEBUG_TYPE_ERROR) {
        fprintf(stderr, "error: %d, severity => %s, message = %s\n", type, severity_string(severity), message);
    } else {
        fprintf(stderr, "warning: %d, severity => %s, message = %s\n", type, severity_string(severity), message);
    }
}

/// Creates a new window and a corresponding OpenGL context
bool display_create(Display *self, char const *title, s32 const width, s32 const height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    self->handle = glfwCreateWindow(width, height, title, NULL, NULL);
    self->running = true;
    self->title = title;
    self->width = width;
    self->height = height;
    self->time = glfwGetTime();

    if (!self->handle) {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(self->handle);
    if (!gladLoadGLLoader((GLADloadproc) (glfwGetProcAddress))) {
        glfwDestroyWindow(self->handle);
        glfwTerminate();
    }

    glfwSwapInterval(1);
    glfwSetInputMode(self->handle, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetFramebufferSizeCallback(self->handle, display_framebuffer_callback);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(display_error_callback, NULL);
    return true;
}

/// Sets the argument that gets passed to every callback
void display_callback_argument(Display const *self, void *arg) {
    glfwSetWindowUserPointer(self->handle, arg);
}

/// Updates the title of the window
void display_title(Display *self, const char *title) {
    glfwSetWindowTitle(self->handle, title);
    self->title = title;
}

/// Destroys the window
void display_destroy(Display const *self) {
    glfwDestroyWindow(self->handle);
    glfwTerminate();
}

/// Swaps front and back buffer
f64 display_update_frame(Display *self) {
    glfwSwapBuffers(self->handle);
    f64 const time = glfwGetTime();
    f64 const frame_time = time - self->time;
    self->time = time;
    return frame_time;
}

/// Polls for incoming events
void display_update_input(Display *self) {
    glfwPollEvents();
    glfwGetWindowSize(self->handle, &self->width, &self->height);
}

/// Checks if the window should be closed or not
bool display_running(Display const *self) {
    return self->running && !glfwWindowShouldClose(self->handle);
}

/// Sets the key callback for the display
void display_key_callback(Display const *self, GLFWkeyfun const callback) {
    glfwSetKeyCallback(self->handle, callback);
}

/// Sets the char callback for the display
void display_char_callback(Display const *self, GLFWcharfun const callback) {
    glfwSetCharCallback(self->handle, callback);
}
