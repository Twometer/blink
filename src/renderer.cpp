//
// Created by Twometer on 14 Apr 2022.
//

#include "renderer.hpp"
#include "loader.hpp"
#include "shader_src.hpp"
#include <glm/ext/matrix_clip_space.hpp>

const GLfloat glyph_vertices[6][4] = {
        {0, 0, 0.0, 0.0},
        {0, 1, 0.0, 1.0},
        {1, 1, 1.0, 1.0},

        {0, 0, 0.0, 0.0},
        {1, 1, 1.0, 1.0},
        {1, 0, 1.0, 0.0}
};

renderer::renderer(GLFWwindow *window) : m_window(window), m_font(R"(C:\Windows\Fonts\CascadiaCode.ttf)", 15),
                                         m_glyph_mesh((GLfloat *) glyph_vertices, sizeof(GLfloat) * 6 * 4) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(26.0 / 255.0, 27.0 / 255.0, 38.0 / 255.0, 1.0);
    m_glyph_shader = loader::load_shader(SHADER_GLYPH_VERT, SHADER_GLYPH_FRAG);
    m_document.insert("v ==> v || 'Hello, world!';");
}

renderer::~renderer() = default;

void renderer::draw_frame() {
    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);

    float scale;
    glfwGetWindowContentScale(m_window, &scale, &scale);

    glViewport(0, 0, w, h);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    m_glyph_shader->bind();
    m_glyph_shader->set("mvp_matrix", glm::ortho(0.0f, (float) w, (float) h, 0.0f));
    m_glyph_shader->set("time", (float) glfwGetTime());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_font.atlas().texture_id());

    // TODO: Do instanced rendering
    m_glyph_mesh.bind();
    m_document.render(m_font);
    for (auto &glyph: m_document.shaped_glyphs()) {
        m_glyph_shader->set("pos_rect", glm::vec4(glyph.pos_x, glyph.pos_y, glyph.width, glyph.height));
        m_glyph_shader->set("tex_rect", glm::vec4(glyph.sprite.u, glyph.sprite.v, glyph.sprite.w, glyph.sprite.h));
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}
