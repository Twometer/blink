//
// Created by Twometer on 14 Apr 2022.
//

#ifndef BLINK_RENDERER_HPP
#define BLINK_RENDERER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include "atlas.hpp"
#include "glyph_cache.hpp"

class renderer {
private:
    GLFWwindow *m_window;
    shader *m_basic_shader;
    font m_font;
    atlas m_atlas;
    glyph_cache m_glyph_cache;

public:
    explicit renderer(GLFWwindow *window);

    ~renderer();

    void draw_frame();

};


#endif //BLINK_RENDERER_HPP