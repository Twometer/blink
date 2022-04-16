//
// Created by Twometer on 16 Apr 2022.
//

#include "editor.hpp"
#include "glm/ext/matrix_clip_space.hpp"

editor::editor(const render_context &ctx, const window &window, font &font, double gui_scale)
        : m_ctx(ctx), m_window(window), m_font(font), m_gui_scale(gui_scale) {
}

void editor::on_draw_frame() {
    if (!m_focused) return;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_font.atlas().texture_id());
    m_ctx.rect_mesh->bind();

    auto size = m_window.get_size();
    auto matrix = glm::ortho(0.0f, (float) size.x, (float) size.y, 0.0f);
    glViewport(0, 0, size.x, size.y);
    draw_selection(matrix);
    draw_text(matrix);
    draw_cursor(matrix);
}

void editor::on_char_typed(char32_t chr) {
    if (!m_focused) return;
    erase_selection();
    m_cursor.x += m_document.insert(chr, m_cursor);
    m_selection.set_to(m_cursor);
}

void editor::on_mouse_button(int button, int action) {
    if (!m_focused) return;
    if (button != GLFW_MOUSE_BUTTON_1 || action != GLFW_PRESS) return;

    m_cursor = mouse_pos();
    update_selection(m_window.is_mouse_button_down(GLFW_KEY_LEFT_SHIFT));

    auto now = glfwGetTime();
    if (now - m_last_press < 0.2) {
        on_mouse_doubleclick();
    }
    m_last_press = now;
}

void editor::on_mouse_doubleclick() {
    auto begin = m_document.find_one_of(m_delimiters, m_cursor, -1);
    auto end = m_document.find_one_of(m_delimiters, m_cursor, 1);
    m_selection.set_pos1({begin, m_cursor.y});
    m_selection.set_pos2({end, m_cursor.y});
    m_cursor.x = end;
}

void editor::on_mouse_move() {
    if (!m_focused) return;
    if (m_window.is_mouse_button_down(GLFW_MOUSE_BUTTON_1)) {
        auto pos = mouse_pos();
        m_cursor = pos;
        m_selection.set_pos1(pos);
    }
}

void editor::on_key_press(int key, int mods) {
    if (!m_focused) return;

    if (handle_key_bind(key, mods)) {
        m_document.ensure_lines();
        limit_cursor_pos();
        update_selection(mods & GLFW_MOD_SHIFT);
        limit_selection();
    }
}

cursor_pos editor::mouse_pos() const {
    auto click = m_window.get_mouse_pos() * glm::vec2(m_gui_scale, m_gui_scale);
    if (click.x < 0) click.x = 0;
    if (click.y < 0) click.y = 0;
    click.x -= m_theme.padding;
    click.y -= m_theme.padding;

    return {
            m_document.get_cursor_pos((int) click.x, m_cursor.y),
            std::min((unsigned) m_document.num_lines() - 1, (unsigned) floor(click.y / (float) m_font.line_height()))
    };
}

void editor::limit_cursor_pos() {
    if (m_cursor.y >= m_document.num_lines()) m_cursor.y = m_document.num_lines() - 1;
    if (m_cursor.x > current_line()->length()) m_cursor.x = current_line()->length();
}

void editor::limit_selection() {
    m_selection.limit_y(m_document.lines().size() - 1);
    m_selection.limit_x(m_document.lines().back()->length());
}

void editor::update_selection(bool expand) {
    if (expand) m_selection.set_pos1(m_cursor);
    else m_selection.set_to(m_cursor);
}

void editor::draw_selection(glm::mat4 matrix) {
    auto shader = m_ctx.rect_shader;
    shader->bind();
    shader->set("mvp_matrix", matrix);
    shader->set("color", glm::vec4(1.0, 1.0, 1.0, 0.15));

    auto begin = m_selection.begin();
    auto end = m_selection.end();
    for (unsigned i = begin.y; i <= end.y; i++) {
        unsigned line_len = m_document.lines()[i]->length();
        unsigned start_chr = i == begin.y ? begin.x : 0;
        unsigned end_chr = i == end.y ? end.x : line_len;

        unsigned start_x = m_document.get_pixel_pos({start_chr, i});
        unsigned end_x = m_document.get_pixel_pos({end_chr, i});
        unsigned width = end_x - start_x;
        if (line_len == 0 && i != end.y) width = 5;

        auto line_rect = glm::vec4(start_x + m_theme.padding, i * m_font.line_height() + m_theme.padding, width,
                                   m_font.line_height());
        shader->set("pos_rect", line_rect);
        m_ctx.rect_mesh->draw();
    }
}

void editor::draw_text(glm::mat4 matrix) {
    auto shader = m_ctx.glyph_shader;
    shader->bind();
    shader->set("mvp_matrix", matrix);
    shader->set("time", (float) glfwGetTime());

    m_document.shape(m_font);
    unsigned pos_y = 0;
    unsigned line_idx = 0;
    for (auto &line: m_document.lines()) {
        for (auto &glyph: line->glyphs) {
            shader->set("pos_rect",
                        glm::vec4(glyph.pos_x + m_theme.padding, glyph.pos_y + pos_y + m_theme.padding, glyph.width,
                                  glyph.height));
            shader->set("tex_rect", glm::vec4(glyph.sprite.u, glyph.sprite.v, glyph.sprite.w, glyph.sprite.h));
            m_ctx.rect_mesh->draw();
        }
        line_idx++;
        pos_y += m_font.line_height();
    }
}

void editor::draw_cursor(glm::mat4 matrix) {
    auto shader = m_ctx.cursor_shader;
    shader->bind();
    shader->set("mvp_matrix", matrix);
    shader->set("time", (float) glfwGetTime());

    unsigned cursor_x = m_document.get_pixel_pos(m_cursor);
    if (cursor_x > 0) cursor_x -= 1;
    shader->set("pos_rect",
                glm::vec4(cursor_x + m_theme.padding, m_cursor.y * m_font.line_height() + m_theme.padding, 1,
                          m_font.line_height()));
    m_ctx.rect_mesh->draw();
}

void editor::erase_selection() {
    m_document.remove_range(m_selection);
    m_selection.collapse_begin();
    m_cursor = m_selection.begin();
}

bool editor::handle_key_bind(int key, int mods) {
    if (key == GLFW_KEY_LEFT) {
        if (mods & GLFW_MOD_CONTROL && m_cursor.x > 0) {
            m_cursor.x = m_document.find_one_of(m_delimiters, m_cursor, -1);
        } else {
            if (m_cursor.x == 0 && m_cursor.y > 0) {
                m_cursor.y--;
                m_cursor.x = current_line()->length();
            } else if (m_cursor.x > 0) {
                m_cursor.x--;
            }
        }
        return true;
    }

    if (key == GLFW_KEY_RIGHT) {
        if (mods & GLFW_MOD_CONTROL && m_cursor.x < current_line()->length()) {
            m_cursor.x = m_document.find_one_of(m_delimiters, m_cursor, 1);
        } else {
            m_cursor.x++;
            if (m_cursor.x > current_line()->length() && m_cursor.y + 1 < m_document.lines().size()) {
                m_cursor.y++;
                m_cursor.x = 0;
            }
        }
        return true;
    }

    if (key == GLFW_KEY_DOWN) {
        m_cursor.y++;
        return true;
    }

    if (key == GLFW_KEY_UP) {
        if (m_cursor.y > 0) m_cursor.y--;
        return true;
    }

    if (key == GLFW_KEY_BACKSPACE) {
        if (!m_selection.empty()) {
            m_document.remove_range(m_selection);
            m_selection.collapse_begin();
            m_cursor = m_selection.begin();
        } else if (mods & GLFW_MOD_CONTROL) {
            unsigned dst = m_document.find_one_of(m_delimiters, m_cursor, -1);
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
        return true;
    }

    if (key == GLFW_KEY_DELETE) {
        if (!m_selection.empty()) {
            m_document.remove_range(m_selection);
            m_selection.collapse_begin();
            m_cursor = m_selection.begin();
        } else if (mods & GLFW_MOD_CONTROL) {
            unsigned dst = m_document.find_one_of(m_delimiters, m_cursor, 1);
            m_document.remove(m_cursor, dst - m_cursor.x);
        } else if (mods & GLFW_MOD_SHIFT) {
            m_document.erase_line(m_cursor.y);
        } else {
            m_document.remove(m_cursor);
        }
        return true;
    }

    if (key == GLFW_KEY_HOME) {
        m_cursor.x = 0;
        if (mods & GLFW_MOD_CONTROL) {
            m_cursor.y = 0;
        }
        return true;
    }

    if (key == GLFW_KEY_END) {
        if (mods & GLFW_MOD_CONTROL) {
            m_cursor.y = m_document.lines().size() - 1;
        }
        m_cursor.x = current_line()->length();
        return true;
    }

    if (key == GLFW_KEY_ENTER || key == GLFW_KEY_KP_ENTER) {
        m_document.insert_split_line(m_cursor);
        m_cursor.y++;
        m_cursor.x = 0;
        return true;
    }

    if (key == GLFW_KEY_TAB) {
        auto num = 4 - (m_cursor.x % 4);
        for (int i = 0; i < num; i++)
            m_cursor.x += m_document.insert(" ", m_cursor);
        return true;
    }

    if (key == GLFW_KEY_C && mods & GLFW_MOD_CONTROL && !m_selection.empty()) {
        m_window.set_clipboard_content(m_document.to_utf8_string(m_selection));
        return true;
    }

    if (key == GLFW_KEY_X && mods & GLFW_MOD_CONTROL) {
        m_window.set_clipboard_content(m_document.to_utf8_string(m_selection));
        erase_selection();
        return true;
    }

    if (key == GLFW_KEY_V && mods & GLFW_MOD_CONTROL) {
        m_cursor.x += m_document.insert(m_window.get_clipboard_content(), m_cursor);
        return true;
    }

    if (key == GLFW_KEY_A && mods & GLFW_MOD_CONTROL) {
        m_selection.set_pos1({0, 0});
        m_selection.set_pos2({UINT32_MAX, UINT32_MAX});
        limit_selection();
        m_cursor = m_selection.end();
        return true;
    }

    return false;
}
