#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "display.h"
#include "utility.h"

static void display_framebuffer_callback(GLFWwindow* handle, s32 width, s32 height) {
    glViewport(0, 0, width, height);
}

static const char* severity_string(u32 severity) {
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

static void
display_error_callback(u32 source, u32 type, u32 id, u32 severity, s32 length, const char* message, const void* user) {
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
        return;
    }

    if (type == GL_DEBUG_TYPE_ERROR) {
        fprintf(stderr, "error: %d, severity => %s, message = %s\n", type, severity_string(severity), message);
    } else {
        fprintf(stderr, "warning: %d, severity => %s, message = %s\n", type, severity_string(severity), message);
    }
}

bool display_create(display_t* self, const char* title, s32 width, s32 height) {
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

    glfwSwapInterval(0);
    glfwSetInputMode(self->handle, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetFramebufferSizeCallback(self->handle, display_framebuffer_callback);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(display_error_callback, NULL);
    return true;
}

void display_title(display_t* self, const char* title) {
    glfwSetWindowTitle(self->handle, title);
    self->title = title;
}

void display_destroy(display_t* self) {
    glfwDestroyWindow(self->handle);
    glfwTerminate();
}

f64 display_update_frame(display_t* self) {
    glfwSwapBuffers(self->handle);
    f64 time = glfwGetTime();
    f64 frame_time = time - self->time;
    self->time = time;
    return frame_time;
}

void display_update_input(display_t* self) {
    glfwPollEvents();
    glfwGetWindowSize(self->handle, &self->width, &self->height);
}

bool display_running(display_t* self) {
    return self->running && !glfwWindowShouldClose(self->handle);
}

void display_callback_argument(display_t* self, void* arg) {
    glfwSetWindowUserPointer(self->handle, arg);
}

void display_key_callback(display_t* self, GLFWkeyfun callback) {
    glfwSetKeyCallback(self->handle, callback);
}

void display_char_callback(display_t* self, GLFWcharfun callback) {
    glfwSetCharCallback(self->handle, callback);
}