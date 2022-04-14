//
// Created by Twometer on 13 Apr 2022.
//

#ifndef BLINK_FONT_HPP
#define BLINK_FONT_HPP

#include <map>
#include <string>
#include <freetype/freetype.h>
#include <hb.h>
#include "atlas.hpp"

struct glyph_info {
    atlas_sprite sprite;
    long bearing_x;
    long bearing_y;
    unsigned width;
    unsigned height;
};

class font {
private:
    hb_blob_t *m_hb_blob = nullptr;
    hb_face_t *m_hb_face = nullptr;
    hb_font_t *m_hb_font = nullptr;
    FT_Face m_ft_face = nullptr;
    std::map<int, glyph_info> m_glyphs;
    atlas m_atlas;

public:
    explicit font(const std::string &path, int size);

    ~font();

    const glyph_info &get_glyph_info(int codepoint);

    [[nodiscard]] inline const atlas &atlas() const { return m_atlas; }

    [[nodiscard]] inline hb_font_t *hb_font() const { return m_hb_font; }

    [[nodiscard]] inline FT_Face ft_face() const { return m_ft_face; }

};


#endif //BLINK_FONT_HPP
