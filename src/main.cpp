#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <hb.h>
#include <freetype/freetype.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <vector>
#include <chrono>

#include "shader.hpp"
#include "shader_src.hpp"
#include "loader.hpp"
#include "font.hpp"

shader *basic_shader;

struct RenderGlyph {
    GLuint textureId;
    double x;
    double y;
    double w;
    double h;
};

std::vector<RenderGlyph> render_glyphs{};
GLuint vao;
GLuint vbo;

auto startup_time = std::chrono::steady_clock::now();

float the_time() {
    auto time_elapsed = std::chrono::steady_clock::now() - startup_time;
    return (float) std::chrono::duration_cast<std::chrono::milliseconds>(time_elapsed).count() / 1000.0f;
}

void redraw(GLFWwindow *window) {
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);

    glViewport(0, 0, w, h);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    basic_shader->set("mvpMatrix", glm::ortho(0.0f, (float) w, (float) h, 0.0f));
    basic_shader->set("time", the_time());

    int text_x = 32;
    int text_y = 32;

    for (auto &glyph: render_glyphs) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, glyph.textureId);
        basic_shader->set("rect", glm::vec4(glyph.x + text_x, glyph.y + text_y, glyph.w, glyph.h));
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }


    glfwSwapBuffers(window);
}

void font_test() {
    auto fontFile = R"(C:\Windows\Fonts\CascadiaCode.ttf)";
    //auto fontFile = R"(C:\Windows\Fonts\Consola.ttf)";


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    font font(fontFile, 15);

    hb_buffer_t *buf;
    buf = hb_buffer_create();
    hb_buffer_add_utf8(buf, "/* beautiful text rendering */ (a, b) => { a === b && c != d }", -1, 0, -1);

    hb_buffer_set_direction(buf, HB_DIRECTION_LTR);
    hb_buffer_set_script(buf, HB_SCRIPT_LATIN);
    hb_buffer_set_language(buf, hb_language_from_string("en", -1));

    hb_shape(font.hb_font(), buf, nullptr, 0);

    unsigned int glyph_count;
    hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos(buf, &glyph_count);
    hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(buf, &glyph_count);


    hb_position_t cursor_x = 0;
    hb_position_t cursor_y = 0;
    for (unsigned int i = 0; i < glyph_count; i++) {
        hb_codepoint_t glyphid = glyph_info[i].codepoint;
        hb_position_t x_offset = glyph_pos[i].x_offset;
        hb_position_t y_offset = glyph_pos[i].y_offset;
        hb_position_t x_advance = glyph_pos[i].x_advance;
        hb_position_t y_advance = glyph_pos[i].y_advance;

        if (FT_Load_Glyph(font.ft_face(), glyphid, FT_LOAD_RENDER)) {
            std::cerr << "FAIL LOAD " << glyphid << std::endl;
        } else {
            auto pos_x = (cursor_x + x_offset) / 64.0;
            auto pos_y = (cursor_y + y_offset) / 64.0;
            auto glyph = font.ft_face()->glyph;

            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, glyph->bitmap.width, glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                         glyph->bitmap.buffer);
            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            auto horizontalOffset = glyph->metrics.horiBearingX / 64;
            auto verticalOffset = glyph->metrics.horiBearingY / 64;

            render_glyphs.push_back({texture, pos_x + horizontalOffset, pos_y - verticalOffset + 32,
                                     (double) glyph->bitmap.width,
                                     (double) glyph->bitmap.rows});
        }
        cursor_x += x_advance;
        cursor_y += y_advance;
    }

    hb_buffer_destroy(buf);


    GLfloat vertexData[6][4] = {
            {0, 0, 0.0, 0.0},
            {0, 1, 0.0, 1.0},
            {1, 1, 1.0, 1.0},

            {0, 0, 0.0, 0.0},
            {1, 1, 1.0, 1.0},
            {1, 0, 1.0, 0.0}
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, vertexData, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindVertexArray(0);
}

int main() {
    float init = the_time();

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 0);

    auto window = glfwCreateWindow(1024, 768, "Blink 0.1.0", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to load OpenGL" << std::endl;
        glfwTerminate();
        return 1;
    }

    float end = the_time();
    std::cout << "Initialized after " << (end - init) * 1000 << "ms" << std::endl;

    glfwSetWindowRefreshCallback(window, redraw);

    basic_shader = loader::load_shader(SHADER_BASIC_VERT, SHADER_BASIC_FRAG);
    basic_shader->bind();
    font_test();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(26.0 / 255.0, 27.0 / 255.0, 38.0 / 255.0, 1.0);
    while (!glfwWindowShouldClose(window)) {
        redraw(window);
        glfwPollEvents();
    }

    return 0;
}


