//
// Created by Twometer on 14 Apr 2022.
//

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "window.hpp"
#include "renderer.hpp"
#include "imgui_style.hpp"

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
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    the_renderer->on_draw_frame();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

void mouse_cb(GLFWwindow *, double, double) {
    the_renderer->on_mouse_move();
}

void click_cb(GLFWwindow *, int button, int action, int) {
    the_renderer->on_mouse_button(button, action);
}

void char_cb(GLFWwindow *window, unsigned codepoint) {
    the_renderer->on_char_typed((char32_t) codepoint);
    draw_frame(window);
}

void key_cb(GLFWwindow *, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
        the_renderer->on_key_press(key, mods);
}

int main() {
    auto glfw_window = create_window();
    glfwSetWindowRefreshCallback(glfw_window, draw_frame);
    glfwSetCharCallback(glfw_window, char_cb);
    glfwSetKeyCallback(glfw_window, key_cb);
    glfwSetMouseButtonCallback(glfw_window, click_cb);
    glfwSetCursorPosCallback(glfw_window, mouse_cb);

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(glfw_window, true);
    ImGui_ImplOpenGL3_Init("#version 100");
    apply_imgui_style();

    window win(glfw_window);
    the_renderer = new renderer(win);
    while (!glfwWindowShouldClose(glfw_window)) {
        draw_frame(glfw_window);
        glfwPollEvents();
    }
    delete the_renderer;
    ImGui_ImplGlfw_Shutdown();
    glfwDestroyWindow(glfw_window);
    glfwTerminate();
}