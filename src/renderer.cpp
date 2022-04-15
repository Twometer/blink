//
// Created by Twometer on 14 Apr 2022.
//

#include "renderer.hpp"
#include "loader.hpp"
#include "shader_src.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>

const GLfloat rect_vertices[6][4] = {
        {0, 0, 0.0, 0.0},
        {0, 1, 0.0, 1.0},
        {1, 1, 1.0, 1.0},

        {0, 0, 0.0, 0.0},
        {1, 1, 1.0, 1.0},
        {1, 0, 1.0, 0.0}
};

renderer::renderer(GLFWwindow *window) : m_window(window), m_font(R"(C:\Windows\Fonts\Arial.ttf)", 15),
                                         m_rect_mesh((GLfloat *) rect_vertices, sizeof(GLfloat) * 6 * 4) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(26.0 / 255.0, 27.0 / 255.0, 38.0 / 255.0, 1.0);
    m_glyph_shader = loader::load_shader(SHADER_RECT_VERT, SHADER_GLYPH_FRAG);
    m_cursor_shader = loader::load_shader(SHADER_RECT_VERT, SHADER_CURSOR_FRAG);
    m_cursor_pos += m_document.insert("v ==> v || 'Hello, world!';");
}

renderer::~renderer() = default;

void renderer::on_draw_frame() {
    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);

    float scale;
    glfwGetWindowContentScale(m_window, &scale, &scale);

    GLFWcursor *cur = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    glfwSetCursor(m_window, cur);

    glViewport(0, 0, w, h);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    auto matrix = glm::ortho(0.0f, (float) w, (float) h, 0.0f);

    m_glyph_shader->bind();
    m_glyph_shader->set("mvp_matrix", matrix);
    m_glyph_shader->set("time", (float) glfwGetTime());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_font.atlas().texture_id());

    // TODO: Do instanced rendering
    m_rect_mesh.bind();

    m_document.shape(m_font);
    /*for (auto &glyph: m_document.shaped_glyphs()) {
        m_glyph_shader->set("pos_rect", glm::vec4(glyph.pos_x, glyph.pos_y, glyph.width, glyph.height));
        m_glyph_shader->set("tex_rect", glm::vec4(glyph.sprite.u, glyph.sprite.v, glyph.sprite.w, glyph.sprite.h));
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }*/

    m_cursor_shader->bind();
    m_cursor_shader->set("mvp_matrix", matrix);
    m_cursor_shader->set("time", (float) glfwGetTime());
    unsigned cursor_x = m_document.get_cursor_pos(m_cursor_pos);
    if (cursor_x > 0) cursor_x -= 1;
    m_cursor_shader->set("pos_rect", glm::vec4(cursor_x, 0, 1, m_font.line_height()));
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void renderer::on_char_typed(char chr) {
    m_cursor_pos += m_document.insert(chr, m_cursor_pos);
}

void renderer::on_mouse_click() {

}

void renderer::on_key_press(int key, int mods) {
    static const std::set<char> delimiters{' ', '.', '#', '\'', '"', ';', ':', '[', ']', '{', '}', '<', '>'};
    if (key == GLFW_KEY_LEFT) {
        if (mods & GLFW_MOD_CONTROL) {
            m_cursor_pos = m_document.find_one_of(delimiters, m_cursor_pos, -1);
        } else {
            m_cursor_pos--;
        }
    } else if (key == GLFW_KEY_RIGHT) {
        if (mods & GLFW_MOD_CONTROL) {
            m_cursor_pos = m_document.find_one_of(delimiters, m_cursor_pos, 1);
        } else {
            m_cursor_pos++;
        }
    } else if (key == GLFW_KEY_BACKSPACE) {
        if (mods & GLFW_MOD_CONTROL) {
            unsigned dst = m_document.find_one_of(delimiters, m_cursor_pos, -1);
            unsigned len = m_cursor_pos - dst;
            m_document.remove(m_cursor_pos - len, len);
        } else if (m_cursor_pos > 0) {
            m_cursor_pos--;
            m_document.remove(m_cursor_pos);
        }
    } else if (key == GLFW_KEY_DELETE) {
        if (mods & GLFW_MOD_CONTROL) {
            unsigned dst = m_document.find_one_of(delimiters, m_cursor_pos, 1);
            m_document.remove(m_cursor_pos, dst - m_cursor_pos);
        } else {
            m_document.remove(m_cursor_pos);
        }
    } else if (key == GLFW_KEY_HOME) {
        m_cursor_pos = 0;
    } else if (key == GLFW_KEY_END) {
        m_cursor_pos = m_document.length();
    }

    if (m_cursor_pos < 0) m_cursor_pos = 0;
    if (m_cursor_pos > m_document.length()) m_cursor_pos = m_document.length();
}
