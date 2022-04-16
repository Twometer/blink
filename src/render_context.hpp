//
// Created by Twometer on 16 Apr 2022.
//

#ifndef BLINK_RENDER_CONTEXT_HPP
#define BLINK_RENDER_CONTEXT_HPP

#include "shader.hpp"
#include "mesh.hpp"

struct render_context {
    shader *glyph_shader;
    shader *cursor_shader;
    shader *rect_shader;
    mesh *rect_mesh;

};

#endif //BLINK_RENDER_CONTEXT_HPP
