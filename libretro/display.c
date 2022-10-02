#include "display.h"

bool display_create(display_t* display) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    display->handle = glfwCreateWindow((int) display->width, (int) display->height, display->title, NULL, NULL);
    display->running = true;

    if (!display->handle) {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(display->handle);
    if (!gladLoadGLLoader((GLADloadproc) (glfwGetProcAddress))) {
        glfwDestroyWindow(display->handle);
        glfwTerminate();
    }
    glfwSwapInterval(1);
    return true;
}

void display_destroy(display_t* display) {
    glfwDestroyWindow(display->handle);
    glfwTerminate();
}

void display_update(display_t* display) {
    glfwPollEvents();
}

bool display_running(display_t* display) {
    return display->running && !glfwWindowShouldClose(display->handle);
}