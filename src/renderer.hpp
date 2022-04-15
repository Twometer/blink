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

class renderer {
private:
    GLFWwindow *m_window;
    shader *m_glyph_shader;
    shader *m_cursor_shader;
    font m_font;
    document m_document{};
    mesh m_rect_mesh;
    unsigned m_cursor_pos = 0;

public:
    explicit renderer(GLFWwindow *window);

    ~renderer();

    void on_draw_frame();

    void on_char_typed(char chr);

    void on_mouse_click();

    void on_key_press(int key, int mods);

};


#endif //BLINK_RENDERER_HPP
