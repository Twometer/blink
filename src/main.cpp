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
    the_renderer->draw_frame();
    glfwSwapBuffers(window);
}

int main() {
    auto window = create_window();
    glfwSetWindowRefreshCallback(window, draw_frame);

    the_renderer = new renderer(window);
    while (!glfwWindowShouldClose(window)) {
        draw_frame(window);
        glfwPollEvents();
    }
    delete the_renderer;
    glfwTerminate();
}