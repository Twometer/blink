//
// Created by Twometer on 13 Apr 2022.
//

#include <stdexcept>
#include "font.hpp"
#include "freetype.hpp"
#include "loader.hpp"

static void destroy_hb_file(void *file) {
    delete[] (uint8_t *) file;
}

font::font(const std::string &path, int size) : size(size) {
    auto font_data = loader::load_file(path);

    const int font_scale = size * 64;

    // Setup FreeType font
    if (FT_New_Memory_Face(freetype::get(), font_data.data, (long) font_data.size, 0, &m_ft_face)) {
        throw std::runtime_error("Failed to initialize FreeType font");
    }
    FT_Set_Char_Size(m_ft_face, font_scale, font_scale, 0, 0);

    // Setup HarfBuzz font
    m_hb_blob = hb_blob_create((const char *) font_data.data, font_data.size, HB_MEMORY_MODE_READONLY,
                                     font_data.data, destroy_hb_file);
    m_hb_face = hb_face_create(m_hb_blob, 0);
    m_hb_font = hb_font_create(m_hb_face);
    hb_font_set_scale(m_hb_font, font_scale, font_scale);
}

font::~font() {
    hb_font_destroy(m_hb_font);
    hb_face_destroy(m_hb_face);
    hb_blob_destroy(m_hb_blob);
}
