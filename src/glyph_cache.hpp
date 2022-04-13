//
// Created by Twometer on 14 Apr 2022.
//

#ifndef BLINK_GLYPH_CACHE_HPP
#define BLINK_GLYPH_CACHE_HPP


#include "font.hpp"
#include "atlas.hpp"

struct glyph_info {
    atlas_sprite sprite;
    long bearing_x;
    long bearing_y;
};

class glyph_cache {

private:
    font *m_font;
    atlas *m_atlas;
    std::map<int, glyph_info> m_glyphs{};

public:
    glyph_cache(font *font, atlas *atlas);

    const glyph_info &get_glyph_info(int codepoint);
};


#endif //BLINK_GLYPH_CACHE_HPP
