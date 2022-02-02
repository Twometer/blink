#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <hb.h>
#include <freetype/freetype.h>

void redraw(GLFWwindow *window) {
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);

    glViewport(0, 0, w, h);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
}

void font_test() {
    auto fontFile = R"(C:\Windows\Fonts\CascadiaCode.ttf)";

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Failed to initialize FreeType" << std::endl;
        return;
    }

    FT_Face ftFace;
    if (FT_New_Face(ft, fontFile, 0, &ftFace)) {
        std::cerr << "Failed to load FT font" << std::endl;
        return;
    }
    FT_Set_Pixel_Sizes(ftFace, 0, 32);

    hb_buffer_t *buf;
    buf = hb_buffer_create();
    hb_buffer_add_utf8(buf, "() => 'hello, world';", -1, 0, -1);

    hb_buffer_set_direction(buf, HB_DIRECTION_LTR);
    hb_buffer_set_script(buf, HB_SCRIPT_JAVANESE);
    hb_buffer_set_language(buf, hb_language_from_string("en", -1));

    hb_blob_t *blob = hb_blob_create_from_file(fontFile); /* or hb_blob_create_from_file_or_fail() */
    hb_face_t *face = hb_face_create(blob, 0);
    hb_font_t *font = hb_font_create(face);

    hb_shape(font, buf, NULL, 0);

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
        if (FT_Load_Glyph(ftFace, glyphid, FT_LOAD_RENDER)) {
            std::cerr << "FAIL LOAD " << glyphid << std::endl;
        } else {
            auto pos_x = (cursor_x + x_offset) / 64.0;
            auto pos_y = (cursor_y + y_offset) / 64.0;
            std::cout << "Drawing: " << glyphid << " at " << pos_x << "/" << pos_y << std::endl;
        }
        cursor_x += x_advance;
        cursor_y += y_advance;
    }

    hb_buffer_destroy(buf);
    hb_font_destroy(font);
    hb_face_destroy(face);
    hb_blob_destroy(blob);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 8);

    auto window = glfwCreateWindow(1024, 768, "Blink Editor", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to load OpenGL" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwSetWindowRefreshCallback(window, redraw);

    font_test();

    glClearColor(26.0 / 255.0, 27.0 / 255.0, 38.0 / 255.0, 1.0);
    while (!glfwWindowShouldClose(window)) {
        redraw(window);
        glfwPollEvents();
    }

    return 0;
}


