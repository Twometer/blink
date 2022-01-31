#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void redraw(GLFWwindow *window) {
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);

    glViewport(0, 0, w, h);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 8);

    auto window = glfwCreateWindow(1024, 768, "Blink Editor", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to load OpenGL" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwSetWindowRefreshCallback(window, redraw);

    glClearColor(26.0 / 255.0, 27.0 / 255.0, 38.0 / 255.0, 1.0);
    while (!glfwWindowShouldClose(window)) {
        redraw(window);
        glfwPollEvents();
    }

    return 0;
}


