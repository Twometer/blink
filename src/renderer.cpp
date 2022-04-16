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

float get_scale(GLFWwindow *window) {
    float scale;
    glfwGetWindowContentScale(window, &scale, &scale);
    return scale;
}

#ifdef WIN32
#define FONT R"(C:\Windows\Fonts\CascadiaCode.ttf)"
#else
#define FONT R"(/System/Library/Fonts/SFNSMono.ttf)"
#endif

renderer::renderer(GLFWwindow *window) : m_window(window),
                                         m_gui_scale(get_scale(window)),
                                         m_font(FONT, (int) (15 * get_scale(window))),
                                         m_rect_mesh((GLfloat *) rect_vertices, sizeof(GLfloat) * 6 * 4) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(26.0 / 255.0, 27.0 / 255.0, 38.0 / 255.0, 1.0);
    m_glyph_shader = loader::load_shader(SHADER_RECT_VERT, SHADER_GLYPH_FRAG);
    m_cursor_shader = loader::load_shader(SHADER_RECT_VERT, SHADER_CURSOR_FRAG);
    m_cursor.x += m_document.insert("v ==> v || 'Hello, world!';", m_cursor);
}

renderer::~renderer() = default;

void renderer::on_draw_frame() {
    m_fps_manager.on_frame_begin();

    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);

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
    unsigned pos_y = 0;
    for (auto &line: m_document.lines()) {
        for (auto &glyph: line->glyphs) {
            m_glyph_shader->set("pos_rect", glm::vec4(glyph.pos_x, glyph.pos_y + pos_y, glyph.width, glyph.height));
            m_glyph_shader->set("tex_rect", glm::vec4(glyph.sprite.u, glyph.sprite.v, glyph.sprite.w, glyph.sprite.h));
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        pos_y += m_font.line_height();
    }

    m_cursor_shader->bind();
    m_cursor_shader->set("mvp_matrix", matrix);
    m_cursor_shader->set("time", (float) glfwGetTime());
    unsigned cursor_x = m_document.get_pixel_pos(m_cursor);
    if (cursor_x > 0) cursor_x -= 1;
    m_cursor_shader->set("pos_rect", glm::vec4(cursor_x, m_cursor.y * m_font.line_height(), 1, m_font.line_height()));
    glDrawArrays(GL_TRIANGLES, 0, 6);

    m_fps_manager.on_frame_end();
}

void renderer::on_char_typed(char32_t chr) {
    m_cursor.x += m_document.insert(chr, m_cursor);
}

void renderer::on_mouse_click() {
    double click_x, click_y;
    glfwGetCursorPos(m_window, &click_x, &click_y);
    click_x *= m_gui_scale;
    click_y *= m_gui_scale;
    m_cursor.y = std::min((int) m_document.lines().size() - 1, (int) floor(click_y / m_font.line_height()));
    m_cursor.x = m_document.get_cursor_pos((int) click_x, m_cursor.y);
    normalize_cursor_pos();
}

void renderer::on_key_press(int key, int mods) {
    static const std::set<char32_t> delimiters{' ', '.', '#', '\'', '"', ';', ':', '[', ']', '{', '}', '<', '>'};
    if (key == GLFW_KEY_LEFT) {
        if (mods & GLFW_MOD_CONTROL && m_cursor.x > 0) {
            m_cursor.x = m_document.find_one_of(delimiters, m_cursor, -1);
        } else {
            if (m_cursor.x == 0 && m_cursor.y > 0) {
                m_cursor.y--;
                m_cursor.x = current_line()->length();
            } else if (m_cursor.x > 0) {
                m_cursor.x--;
            }
        }
    } else if (key == GLFW_KEY_RIGHT) {
        if (mods & GLFW_MOD_CONTROL && m_cursor.x < current_line()->length()) {
            m_cursor.x = m_document.find_one_of(delimiters, m_cursor, 1);
        } else {
            m_cursor.x++;
            if (m_cursor.x > current_line()->length() && m_cursor.y + 1 < m_document.lines().size()) {
                m_cursor.y++;
                m_cursor.x = 0;
            }
        }
    } else if (key == GLFW_KEY_BACKSPACE) {
        if (mods & GLFW_MOD_CONTROL) {
            unsigned dst = m_document.find_one_of(delimiters, m_cursor, -1);
            unsigned len = m_cursor.x - dst;
            m_document.remove({m_cursor.x - len, m_cursor.y}, len);
        } else if (m_cursor.x > 0) {
            m_cursor.x--;
            m_document.remove(m_cursor);
        } else if (m_cursor.x == 0 && m_cursor.y > 0) {
            m_cursor.y--;
            m_cursor.x = current_line()->length();

            auto erased_line = m_document.lines()[m_cursor.y + 1];
            m_document.insert(erased_line->buffer.data(), {(unsigned) current_line()->length(), m_cursor.y});
            m_document.erase_line(m_cursor.y + 1);
        }
    } else if (key == GLFW_KEY_DELETE) {
        if (mods & GLFW_MOD_CONTROL) {
            unsigned dst = m_document.find_one_of(delimiters, m_cursor, 1);
            m_document.remove(m_cursor, dst - m_cursor.x);
        } else if (mods & GLFW_MOD_SHIFT) {
            m_document.erase_line(m_cursor.y);
        } else {
            m_document.remove(m_cursor);
        }
    } else if (key == GLFW_KEY_HOME) {
        m_cursor.x = 0;
        if (mods & GLFW_MOD_CONTROL) {
            m_cursor.y = 0;
        }
    } else if (key == GLFW_KEY_END) {
        if (mods & GLFW_MOD_CONTROL) {
            m_cursor.y = m_document.lines().size() - 1;
        }
        m_cursor.x = current_line()->length();
    } else if (key == GLFW_KEY_ENTER) {
        m_document.insert_split_line(m_cursor);
        m_cursor.y++;
        m_cursor.x = 0;
    } else if (key == GLFW_KEY_DOWN) {
        m_cursor.y++;
    } else if (key == GLFW_KEY_UP) {
        if (m_cursor.y > 0) m_cursor.y--;
    } else if (key == GLFW_KEY_V && mods & GLFW_MOD_CONTROL) {
        const char *cb = glfwGetClipboardString(m_window);
        m_cursor.x += m_document.insert(cb, m_cursor);
    } else if (key == GLFW_KEY_TAB) {
        auto num = 4 - (m_cursor.x % 4);
        for (int i = 0; i < num; i++)
            m_cursor.x += m_document.insert(" ", m_cursor);
    }

    normalize_cursor_pos();
}

void renderer::normalize_cursor_pos() {
    if (m_cursor.y >= m_document.lines().size()) m_cursor.y = m_document.lines().size() - 1;
    if (m_cursor.x > current_line()->length()) m_cursor.x = current_line()->length();
}
