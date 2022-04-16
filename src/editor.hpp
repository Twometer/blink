//
// Created by Twometer on 16 Apr 2022.
//

#ifndef BLINK_EDITOR_HPP
#define BLINK_EDITOR_HPP

#include <functional>
#include "range.hpp"
#include "cursor.hpp"
#include "document.hpp"
#include "theme.hpp"
#include "window.hpp"
#include "render_context.hpp"

class editor {

private:
    const render_context &m_ctx;
    const window &m_window;
    font &m_font;
    theme m_theme{};
    range m_selection{};
    document m_document{};
    cursor_pos m_cursor{};

    bool m_focused = false;
    double m_gui_scale = 1.0;

    double m_last_press = 0.0;

    const std::set<char32_t> m_delimiters{' ', '.', '#', '\'', '"', ';', ':', '[', ']', '{', '}', '<', '>'};
public:
    editor(const render_context &ctx, const window &window, font &font, double gui_scale);

    void on_draw_frame();

    void on_char_typed(char32_t chr);

    void on_mouse_button(int button, int action);

    void on_mouse_move();

    void on_key_press(int key, int mods);

    inline void set_focused(bool focused) { m_focused = focused; };

    [[nodiscard]] inline const document &document() const { return m_document; };

private:
    void on_mouse_doubleclick();

    [[nodiscard]] line *current_line() const { return m_document.lines()[m_cursor.y]; }

    [[nodiscard]] cursor_pos mouse_pos() const;

    void limit_cursor_pos();

    void limit_selection();

    void update_selection(bool expand);

    void draw_selection(glm::mat4 matrix);

    void draw_text(glm::mat4 matrix);

    void draw_cursor(glm::mat4 matrix);

    void erase_selection();

    bool handle_key_bind(int key, int mods);
};


#endif //BLINK_EDITOR_HPP
