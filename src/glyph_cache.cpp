//
// Created by Twometer on 14 Apr 2022.
//

#include <stdexcept>
#include "glyph_cache.hpp"

glyph_cache::glyph_cache(font *font, atlas *atlas) : m_font(font), m_atlas(atlas) {}

const glyph_info &glyph_cache::get_glyph_info(int codepoint) {
    if (m_glyphs.contains(codepoint))
        return m_glyphs.at(codepoint);
    else {
        auto face = m_font->ft_face();
        if (FT_Load_Glyph(face, codepoint, FT_LOAD_RENDER)) {
            throw std::runtime_error("Failed to load glyph");
        }

        auto &glyph = face->glyph;
        auto &sprite = m_atlas->insert(codepoint, (int) glyph->bitmap.width, (int) glyph->bitmap.rows,
                                       glyph->bitmap.buffer);

        auto bearing_x = glyph->metrics.horiBearingX / 64;
        auto bearing_y = glyph->metrics.horiBearingY / 64;

        return m_glyphs[codepoint] = {sprite, bearing_x, bearing_y};
    }
}
