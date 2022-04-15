//
// Created by Twometer on 14 Apr 2022.
//

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderer.hpp"

renderer *the_renderer;

GLFWwindow *create_window() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4);

    auto window = glfwCreateWindow(1280, 768, "Blink 0.1.0", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw std::runtime_error("Failed to load OpenGL");
    }

    return window;
}

void draw_frame(GLFWwindow *window) {
    the_renderer->on_draw_frame();
    glfwSwapBuffers(window);
}

void click_cb(GLFWwindow *, int button, int action, int ) {
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
        the_renderer->on_mouse_click();
}

void char_cb(GLFWwindow *window, unsigned codepoint) {
    the_renderer->on_char_typed((char) codepoint);
    draw_frame(window);
}

void key_cb(GLFWwindow *, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
        the_renderer->on_key_press(key, mods);
}

int main() {
    auto window = create_window();
    glfwSetWindowRefreshCallback(window, draw_frame);
    glfwSetCharCallback(window, char_cb);
    glfwSetKeyCallback(window, key_cb);
    glfwSetMouseButtonCallback(window, click_cb);

    the_renderer = new renderer(window);
    while (!glfwWindowShouldClose(window)) {
        draw_frame(window);
        glfwPollEvents();
    }
    delete the_renderer;
    glfwTerminate();
}