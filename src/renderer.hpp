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
    font m_font;
    document m_document{};
    mesh m_glyph_mesh;

public:
    explicit renderer(GLFWwindow *window);

    ~renderer();

    void draw_frame();
};


#endif //BLINK_RENDERER_HPP
