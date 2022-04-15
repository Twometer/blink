//
// Created by Twometer on 14 Apr 2022.
//

#ifndef BLINK_TEXT_BUFFER_HPP
#define BLINK_TEXT_BUFFER_HPP

#include <vector>
#include <string>
#include <hb.h>
#include "font.hpp"

struct shaped_glyph {
    unsigned cluster_idx;
    unsigned codepoint;
    int pos_x;
    int pos_y;
    unsigned width;
    unsigned height;
    atlas_sprite sprite;
};

class text_buffer {
private:
    hb_buffer_t *m_buffer;

public:
    text_buffer();

    ~text_buffer();

    void add_text(const std::string &data);

    void clear();

    std::vector<shaped_glyph> shape(font &font, int x_base = 0, int y_base = 0);
};


#endif //BLINK_TEXT_BUFFER_HPP
