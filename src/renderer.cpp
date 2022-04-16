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
    m_selection.set_to(m_cursor);
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

    // selection
    m_cursor_shader->bind();
    m_cursor_shader->set("time", 0.1f);
    auto begin = m_selection.begin();
    auto end = m_selection.end();
    for (unsigned i = begin.y; i <= end.y; i++) {
        unsigned line_len = m_document.lines()[i]->length();
        unsigned start_x = i == begin.y ? begin.x : 0;
        unsigned end_x = i == end.y ? end.x : line_len;
        unsigned start_x_px = m_document.get_pixel_pos({start_x, i});
        unsigned end_x_px = m_document.get_pixel_pos({end_x, i});
        unsigned width = end_x_px - start_x_px;
        if (line_len == 0 && i != end.y) width = 5;
        m_cursor_shader->set("pos_rect",
                             glm::vec4(start_x_px, i * m_font.line_height(), width, m_font.line_height()));
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // text
    m_glyph_shader->bind();
    m_glyph_shader->set("mvp_matrix", matrix);
    m_glyph_shader->set("time", (float) glfwGetTime());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_font.atlas().texture_id());

    // TODO: Do instanced rendering
    m_rect_mesh.bind();

    m_document.shape(m_font);
    unsigned pos_y = 0;
    unsigned line_idx = 0;
    for (auto &line: m_document.lines()) {
        for (auto &glyph: line->glyphs) {
            m_glyph_shader->bind();
            m_glyph_shader->set("pos_rect", glm::vec4(glyph.pos_x, glyph.pos_y + pos_y, glyph.width, glyph.height));
            m_glyph_shader->set("tex_rect", glm::vec4(glyph.sprite.u, glyph.sprite.v, glyph.sprite.w, glyph.sprite.h));
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        line_idx++;
        pos_y += m_font.line_height();
    }

    // cursor
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
    if (!m_selection.empty()) {
        m_document.remove_range(m_selection);
        m_selection.collapse_begin();
        m_cursor = m_selection.begin();
    }
    m_cursor.x += m_document.insert(chr, m_cursor);
    m_selection.set_to(m_cursor);
}

static const std::set<char32_t> delimiters{' ', '.', '#', '\'', '"', ';', ':', '[', ']', '{', '}', '<', '>'};

void renderer::on_mouse_button(int action) {
    auto pos = mouse_pos();
    if (action == GLFW_PRESS) {
        m_cursor = pos;
        if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            m_selection.set_pos1(pos);
        else
            m_selection.set_to(pos);

        auto now = glfwGetTime();
        if (now - m_last_press < 0.2) {
            auto beg = m_document.find_one_of(delimiters, m_cursor, -1);
            auto end = m_document.find_one_of(delimiters, m_cursor, 1);
            m_selection.set_pos1({beg, m_cursor.y});
            m_selection.set_pos2({end, m_cursor.y});
            m_cursor.x = end;
        }
        m_last_press = now;
    }
}

void renderer::on_mouse_move() {
    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
        auto pos = mouse_pos();
        m_cursor = pos;
        m_selection.set_pos1(pos);
    }
}

void renderer::on_key_press(int key, int mods) {
    bool any_key = false;
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
        any_key = true;
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
        any_key = true;
    } else if (key == GLFW_KEY_BACKSPACE) {
        if (!m_selection.empty()) {
            m_document.remove_range(m_selection);
            m_selection.collapse_begin();
            m_cursor = m_selection.begin();
        } else if (mods & GLFW_MOD_CONTROL) {
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
        any_key = true;
    } else if (key == GLFW_KEY_DELETE) {
        if (!m_selection.empty()) {
            m_document.remove_range(m_selection);
            m_selection.collapse_begin();
            m_cursor = m_selection.begin();
        } else if (mods & GLFW_MOD_CONTROL) {
            unsigned dst = m_document.find_one_of(delimiters, m_cursor, 1);
            m_document.remove(m_cursor, dst - m_cursor.x);
        } else if (mods & GLFW_MOD_SHIFT) {
            m_document.erase_line(m_cursor.y);
        } else {
            m_document.remove(m_cursor);
        }
        any_key = true;
    } else if (key == GLFW_KEY_HOME) {
        m_cursor.x = 0;
        if (mods & GLFW_MOD_CONTROL) {
            m_cursor.y = 0;
        }
        any_key = true;
    } else if (key == GLFW_KEY_END) {
        if (mods & GLFW_MOD_CONTROL) {
            m_cursor.y = m_document.lines().size() - 1;
        }
        m_cursor.x = current_line()->length();
        any_key = true;
    } else if (key == GLFW_KEY_ENTER || key == GLFW_KEY_KP_ENTER) {
        m_document.insert_split_line(m_cursor);
        m_cursor.y++;
        m_cursor.x = 0;
        any_key = true;
    } else if (key == GLFW_KEY_DOWN) {
        m_cursor.y++;
        any_key = true;
    } else if (key == GLFW_KEY_UP) {
        if (m_cursor.y > 0) m_cursor.y--;
        any_key = true;
    } else if (key == GLFW_KEY_V && mods & GLFW_MOD_CONTROL) {
        const char *cb = glfwGetClipboardString(m_window);
        m_cursor.x += m_document.insert(cb, m_cursor);
        any_key = true;
    } else if (key == GLFW_KEY_TAB) {
        auto num = 4 - (m_cursor.x % 4);
        for (int i = 0; i < num; i++)
            m_cursor.x += m_document.insert(" ", m_cursor);
        any_key = true;
    }

    if (any_key) {
        m_document.ensure_lines();
        normalize_cursor_pos();
        update_selection(mods);
    }
}

void renderer::normalize_cursor_pos() {
    if (m_cursor.y >= m_document.lines().size()) m_cursor.y = m_document.lines().size() - 1;
    if (m_cursor.x > current_line()->length()) m_cursor.x = current_line()->length();
}

cursor_pos renderer::mouse_pos() const {
    double click_x, click_y;
    glfwGetCursorPos(m_window, &click_x, &click_y);
    click_x *= m_gui_scale;
    click_y *= m_gui_scale;

    if (click_x < 0) click_x = 0;
    if (click_y < 0) click_y = 0;

    return {
            m_document.get_cursor_pos((int) click_x, m_cursor.y),
            std::min((unsigned) m_document.lines().size() - 1, (unsigned) floor(click_y / m_font.line_height()))
    };
}

void renderer::update_selection(int mods) {
    if (mods & GLFW_MOD_SHIFT) {
        m_selection.set_pos1(m_cursor);
    } else m_selection.set_to(m_cursor);
    m_selection.limit_y(m_document.lines().size()-1);
}
