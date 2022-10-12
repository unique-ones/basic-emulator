#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "display.h"
#include "utility.h"

static void display_framebuffer_callback(GLFWwindow* handle, s32 width, s32 height) {
    display_t* display = glfwGetWindowUserPointer(handle);
    display->width = width;
    display->height = height;
    glViewport(0, 0, width, height);
}

static void display_key_callback(GLFWwindow* handle, s32 key, s32 scancode, s32 action, s32 mods) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) {
        return;
    }
    display_t* display = glfwGetWindowUserPointer(handle);
    if (display->input_buffer) {
        input_buffer_t* input = display->input_buffer;
        switch (key) {
            case GLFW_KEY_LEFT:
                input_buffer_advance_cursor(input, -1);
                break;
            case GLFW_KEY_RIGHT:
                input_buffer_advance_cursor(input, 1);
                break;
            case GLFW_KEY_BACKSPACE:
                input_buffer_remove(input);
                break;
            case GLFW_KEY_TAB:
                input_buffer_emplace(input, '\t');
                break;
            case GLFW_KEY_ENTER:
                input->submit = true;
                break;
            default:
                break;
        }
    }
}

static void display_char_callback(GLFWwindow* handle, u32 unicode) {
    display_t* display = glfwGetWindowUserPointer(handle);
    if (display->input_buffer) {
        input_buffer_emplace(display->input_buffer, (char) toupper((char) unicode));
    }
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
    glfwSetWindowUserPointer(self->handle, self);
    glfwSetFramebufferSizeCallback(self->handle, display_framebuffer_callback);
    glfwSetKeyCallback(self->handle, display_key_callback);
    glfwSetCharCallback(self->handle, display_char_callback);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(display_error_callback, NULL);
    return true;
}

void display_input(display_t* self, input_buffer_t* input_buffer) {
    self->input_buffer = input_buffer;
}

void display_title(display_t* self, const char* title) {
    glfwSetWindowTitle(self->handle, title);
    self->title = title;
}

void display_destroy(display_t* self) {
    glfwDestroyWindow(self->handle);
    glfwTerminate();
}

f64 display_update(display_t* self) {
    glfwSwapBuffers(self->handle);
    glfwPollEvents();
    f64 time = glfwGetTime();
    f64 frame_time = time - self->time;
    self->time = time;
    return frame_time;
}

bool display_running(display_t* self) {
    return self->running && !glfwWindowShouldClose(self->handle);
}
