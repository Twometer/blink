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
#include "editor.hpp"

class renderer {
private:
    window &m_window;

    render_context m_render_context{};
    fps_manager m_fps_manager{};
    shader *m_glyph_shader;
    shader *m_cursor_shader;
    shader *m_rect_shader;
    mesh *m_rect_mesh;

    font *m_font;
    editor *m_editor;

public:
    explicit renderer(window &window);

    ~renderer();

    void on_draw_frame();

    void on_char_typed(char32_t chr);

    void on_mouse_button(int button, int action);

    void on_mouse_move();

    void on_key_press(int key, int mods);

private:
    void do_gui();
};


#endif //BLINK_RENDERER_HPP
