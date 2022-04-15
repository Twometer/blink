//
// Created by Twometer on 14 Apr 2022.
//

#include <iostream>
#include "text_buffer.hpp"

text_buffer::text_buffer() {
    m_buffer = hb_buffer_create();
}

text_buffer::~text_buffer() {
    hb_buffer_destroy(m_buffer);
}

void text_buffer::add_text(const std::u32string &data, unsigned offset) {
    if (offset >= m_string.length())
        m_string += data;
    else
        m_string.insert(offset, data);
}

void text_buffer::add_char(char32_t c, unsigned offset) {
    if (offset >= m_string.length())
        m_string += c;
    else
        m_string.insert(offset, 1, c);
}

void text_buffer::remove_text(unsigned offset, unsigned len) {
    m_string.erase(offset, len);
}

std::u32string text_buffer::remove_text_and_get(unsigned int offset, unsigned int len) {
    auto erased = m_string.substr(offset, len);
    remove_text(offset, len);
    return erased;
}

void text_buffer::clear() {
    m_string.clear();
}

std::vector<shaped_glyph> text_buffer::shape(font &font, int x_base, int y_base) {
    reset_hb();
    hb_shape(font.hb_font(), m_buffer, nullptr, 0);

    unsigned int glyph_count;
    hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos(m_buffer, &glyph_count);
    hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(m_buffer, &glyph_count);

    hb_position_t cursor_x = x_base;
    hb_position_t cursor_y = y_base + font.ft_face()->size->metrics.ascender;

    std::vector<shaped_glyph> result;

    for (unsigned int i = 0; i < glyph_count; i++) {
        hb_codepoint_t codepoint = glyph_info[i].codepoint;
        hb_position_t x_offset = glyph_pos[i].x_offset;
        hb_position_t y_offset = glyph_pos[i].y_offset;
        hb_position_t x_advance = glyph_pos[i].x_advance;
        hb_position_t y_advance = glyph_pos[i].y_advance;
        unsigned cluster_idx = glyph_info[i].cluster;

        auto &glyph = font.get_glyph_info((int) codepoint);

        auto pos_x = (cursor_x + x_offset + glyph.bearing_x) / 64;
        auto pos_y = (cursor_y + y_offset - glyph.bearing_y) / 64;

        shaped_glyph shaped_glyph = {cluster_idx, codepoint, (int) pos_x, (int) pos_y, glyph.width, glyph.height,
                                     x_advance / 64u,
                                     glyph.sprite};
        result.push_back(shaped_glyph);

        cursor_x += x_advance;
        cursor_y += y_advance;
    }

    return result;
}

void text_buffer::reset_hb() {
    hb_buffer_clear_contents(m_buffer);
    hb_buffer_add_utf32(m_buffer, (uint32_t *) m_string.data(), (int) m_string.length(), 0, -1);
    hb_buffer_guess_segment_properties(m_buffer);
}





