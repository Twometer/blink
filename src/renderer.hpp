//
// Created by Twometer on 14 Apr 2022.
//

#ifndef BLINK_RENDERER_HPP
#define BLINK_RENDERER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include "atlas.hpp"
#include "text_buffer.hpp"
#include "document.hpp"
#include "mesh.hpp"
#include "fps_manager.hpp"
#include "range.hpp"

class renderer {
private:
    GLFWwindow *m_window;
    shader *m_glyph_shader;
    shader *m_cursor_shader;
    font m_font;
    mesh m_rect_mesh;
    fps_manager m_fps_manager{};
    double m_gui_scale = 0;
    document m_document{};
    range m_selection{};
    cursor_pos m_cursor{};
    double m_last_press = 0;

public:
    explicit renderer(GLFWwindow *window);

    ~renderer();

    void on_draw_frame();

    void on_char_typed(char32_t chr);

    void on_mouse_button(int action);

    void on_mouse_move();

    void on_key_press(int key, int mods);

private:
    [[nodiscard]] line *current_line() const { return m_document.lines()[m_cursor.y]; }

    [[nodiscard]] cursor_pos mouse_pos() const;

    void normalize_cursor_pos();
};


#endif //BLINK_RENDERER_HPP
