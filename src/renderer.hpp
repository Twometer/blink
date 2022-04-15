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

class renderer {
private:
    GLFWwindow *m_window;
    shader *m_glyph_shader;
    shader *m_cursor_shader;
    font m_font;
    document m_document{};
    mesh m_rect_mesh;
    unsigned m_cursor_x = 0;
    unsigned m_cursor_y = 0;
    fps_manager m_fps_manager{};
    double m_gui_scale = 0;

public:
    explicit renderer(GLFWwindow *window);

    ~renderer();

    void on_draw_frame();

    void on_char_typed(char chr);

    void on_mouse_click();

    void on_key_press(int key, int mods);

private:
    [[nodiscard]] line* current_line() const { return m_document.lines()[m_cursor_y]; }

    void normalize_cursor_pos();
};


#endif //BLINK_RENDERER_HPP
