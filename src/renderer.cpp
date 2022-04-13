//
// Created by Twometer on 14 Apr 2022.
//

#include "renderer.hpp"
#include "loader.hpp"
#include "shader_src.hpp"
#include <glm/ext/matrix_clip_space.hpp>

renderer::renderer(GLFWwindow *window) : m_window(window), m_font(R"(C:\Windows\Fonts\CascadiaCode.ttf)", 15),
                                         m_atlas(2048, 16), m_glyph_cache(&m_font, &m_atlas) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(26.0 / 255.0, 27.0 / 255.0, 38.0 / 255.0, 1.0);
    m_basic_shader = loader::load_shader(SHADER_BASIC_VERT, SHADER_BASIC_FRAG);
}

renderer::~renderer() = default;

void renderer::draw_frame() {
    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);

    float scale;
    glfwGetWindowContentScale(m_window, &scale, &scale);

    glViewport(0, 0, w, h);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    m_basic_shader->bind();
    m_basic_shader->set("mvpMatrix", glm::ortho(0.0f, (float) w, (float) h, 0.0f));
    m_basic_shader->set("time", (float) glfwGetTime());
}
