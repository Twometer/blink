//
// Created by Twometer on 14 Apr 2022.
//

#include "renderer.hpp"
#include "loader.hpp"
#include "shader_src.hpp"
#include <glm/ext/matrix_clip_space.hpp>

GLuint vao;
GLuint vbo;

renderer::renderer(GLFWwindow *window) : m_window(window), m_font(R"(C:\Windows\Fonts\CascadiaCode.ttf)", 15) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(26.0 / 255.0, 27.0 / 255.0, 38.0 / 255.0, 1.0);
    m_basic_shader = loader::load_shader(SHADER_BASIC_VERT, SHADER_BASIC_FRAG);

    GLfloat vertexData[6][4] = {
            {0, 0, 0.0, 0.0},
            {0, 1, 0.0, 1.0},
            {1, 1, 1.0, 1.0},

            {0, 0, 0.0, 0.0},
            {1, 1, 1.0, 1.0},
            {1, 0, 1.0, 0.0}
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, vertexData, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindVertexArray(0);
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
    m_basic_shader->set("mvp_matrix", glm::ortho(0.0f, (float) w, (float) h, 0.0f));
    m_basic_shader->set("time", (float) glfwGetTime());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_font.atlas().texture_id());
    glBindVertexArray(vao);

    m_text_buffer.clear();
    m_text_buffer.add_text("v => v || 'Hello, world!';");
    auto result = m_text_buffer.shape(m_font, 0, 0);
    for (auto &glyph: result) {
        m_basic_shader->set("pos_rect", glm::vec4(glyph.pos_x, glyph.pos_y, glyph.width, glyph.height));
        m_basic_shader->set("tex_rect", glm::vec4(glyph.sprite.u, glyph.sprite.v, glyph.sprite.w, glyph.sprite.h));
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glBindVertexArray(0);
}
