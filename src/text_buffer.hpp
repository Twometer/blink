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
    unsigned advance;
    atlas_sprite sprite;
};

class text_buffer {
private:
    std::string m_string;
    hb_buffer_t *m_buffer;

public:
    text_buffer();

    ~text_buffer();

    void add_text(const std::string &data, unsigned offset);

    void add_char(char data, unsigned offset);

    void remove_text(unsigned offset, unsigned len);

    void clear();

    std::vector<shaped_glyph> shape(font &font, int x_base = 0, int y_base = 0);

    [[nodiscard]] inline size_t size() const { return m_string.size(); };

    [[nodiscard]] inline char at(unsigned offset) const { return m_string[offset]; };

private:
    void reset_hb();
};


#endif //BLINK_TEXT_BUFFER_HPP
