//
// Created by Twometer on 15 Apr 2022.
//

#ifndef BLINK_DOCUMENT_HPP
#define BLINK_DOCUMENT_HPP

#include <set>
#include "text_buffer.hpp"

struct line {
    text_buffer buffer{};
    std::vector<shaped_glyph> glyphs;
    bool dirty = true;

public:
    [[nodiscard]] inline size_t length() const { return buffer.size(); };
};

class document {
private:
    std::vector<line *> m_lines{};
    size_t m_length = 0;

public:
    document();

    ~document();

    unsigned insert(const std::string &data, unsigned pos_x = 0, unsigned pos_y = 0);

    unsigned insert(const std::u32string &data, unsigned pos_x = 0, unsigned pos_y = 0);

    unsigned insert(char32_t c, unsigned pos_x = 0, unsigned pos_y = 0);

    void remove(unsigned pos_x, unsigned pos_y = 0, unsigned len = 1);

    void insert_line(unsigned pos_y);

    void insert_split_line(unsigned pos_x, unsigned pos_y);

    void erase_line(unsigned pos_y);

    void shape(font &font);

    unsigned find_one_of(const std::set<char32_t> &c, unsigned pos_x, unsigned pos_y, int direction);

    [[nodiscard]] unsigned get_char_pos(unsigned cursor_pos_x, unsigned line_idx = 0);

    [[nodiscard]] unsigned get_cursor_pos(unsigned char_pos_x, unsigned char_pos_y = 0);

    [[nodiscard]] inline size_t length() const { return m_length; };

    [[nodiscard]] inline const std::vector<line *> &lines() const { return m_lines; };
};


#endif //BLINK_DOCUMENT_HPP
