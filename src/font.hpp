//
// Created by Twometer on 13 Apr 2022.
//

#ifndef BLINK_FONT_HPP
#define BLINK_FONT_HPP


#include <string>
#include <freetype/freetype.h>
#include <hb.h>

class font {
private:
    hb_blob_t *m_hb_blob = nullptr;
    hb_face_t *m_hb_face = nullptr;
    hb_font_t *m_hb_font = nullptr;
    FT_Face m_ft_face = nullptr;
    int size;

public:
    explicit font(const std::string &path, int size);

    ~font();

    inline hb_font_t *hb_font() { return m_hb_font; }

    inline FT_Face ft_face() { return m_ft_face; }

};


#endif //BLINK_FONT_HPP
