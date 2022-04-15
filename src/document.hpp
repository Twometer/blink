//
// Created by Twometer on 15 Apr 2022.
//

#ifndef BLINK_DOCUMENT_HPP
#define BLINK_DOCUMENT_HPP

#include <set>
#include "text_buffer.hpp"

class document {
private:
    text_buffer m_buffer{};
    bool m_dirty = false;
    std::vector<shaped_glyph> m_shaped;
    size_t m_length = 0;

public:
    unsigned  insert(const std::string &data, unsigned offset = 0);

    unsigned  insert(char c, unsigned offset = 0);

    void remove(unsigned offset, unsigned len = 1);

    void shape(font &font);

    unsigned find_one_of(std::set<char> c, unsigned offset, int direction);

    [[nodiscard]] unsigned get_cursor_pos(unsigned text_pos);

    [[nodiscard]] inline size_t length() const { return m_length; };

    [[nodiscard]] inline const std::vector<shaped_glyph> &shaped_glyphs() const { return m_shaped; };
};


#endif //BLINK_DOCUMENT_HPP
