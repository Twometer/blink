//
// Created by Twometer on 15 Apr 2022.
//

#ifndef BLINK_DOCUMENT_HPP
#define BLINK_DOCUMENT_HPP


#include "text_buffer.hpp"

class document {
private:
    text_buffer m_buffer{};
    bool m_dirty = false;
    std::vector<shaped_glyph> m_shaped;

public:
    void insert(const std::string &data, int offset = 0);

    void render(font &font);

    [[nodiscard]] inline const std::vector<shaped_glyph> &shaped_glyphs() const { return m_shaped; };
};


#endif //BLINK_DOCUMENT_HPP
