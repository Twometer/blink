//
// Created by Twometer on 14 Apr 2022.
//

#include "renderer.hpp"
#include "loader.hpp"
#include "shader_src.hpp"
#include "imgui.h"

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

renderer::renderer(window &window) : m_window(window) {
    m_rect_shader = loader::load_shader(SHADER_RECT_VERT, SHADER_RECT_FRAG);
    m_glyph_shader = loader::load_shader(SHADER_RECT_VERT, SHADER_GLYPH_FRAG);
    m_cursor_shader = loader::load_shader(SHADER_RECT_VERT, SHADER_CURSOR_FRAG);
    m_rect_mesh = new mesh((GLfloat *) rect_vertices, sizeof(GLfloat) * 6 * 4);
    m_render_context = {m_glyph_shader, m_cursor_shader, m_rect_shader, m_rect_mesh};

    m_font = new font(FONT, (int) (15 * window.get_scale()));
    m_editor = new editor(m_render_context, m_window, *m_font, window.get_scale());
    m_editor->set_focused(true);

    window.set_cursor(GLFW_IBEAM_CURSOR);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(26.0 / 255.0, 27.0 / 255.0, 38.0 / 255.0, 1.0);
}

renderer::~renderer() {
    delete m_rect_shader;
    delete m_glyph_shader;
    delete m_cursor_shader;
    delete m_rect_mesh;
    delete m_font;
    delete m_editor;
}

void renderer::on_draw_frame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_fps_manager.on_frame_begin();
    m_editor->on_draw_frame();
    m_fps_manager.on_frame_end();
    do_gui();
}

void renderer::on_char_typed(char32_t chr) {
    m_editor->on_char_typed(chr);
}

void renderer::on_mouse_button(int button, int action) {
    m_editor->on_mouse_button(button, action);
}

void renderer::on_mouse_move() {
    m_editor->on_mouse_move();
}

void renderer::on_key_press(int key, int mods) {
    m_editor->on_key_press(key, mods);
}

void renderer::do_gui() {
    // ImGui::Text("Hello, world!");
}
