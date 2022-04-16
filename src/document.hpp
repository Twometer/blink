//
// Created by Twometer on 15 Apr 2022.
//

#ifndef BLINK_DOCUMENT_HPP
#define BLINK_DOCUMENT_HPP

#include <set>
#include "text_buffer.hpp"
#include "cursor.hpp"

struct line {
    text_buffer buffer{};
    std::vector<shaped_glyph> glyphs;
    bool dirty = true;

public:
    [[nodiscard]] inline size_t length() const { return buffer.length(); };
};

class document {
private:
    std::vector<line *> m_lines{};
    size_t m_length = 0;

public:
    document();

    ~document();

    unsigned insert(const std::string &data, cursor_pos pos);

    unsigned insert(const std::u32string &data, cursor_pos pos);

    unsigned insert(char32_t c, cursor_pos pos);

    void remove(cursor_pos pos, unsigned len = 1);

    void insert_line(unsigned pos_y);

    void insert_split_line(cursor_pos pos);

    void erase_line(unsigned pos_y);

    void shape(font &font);

    unsigned find_one_of(const std::set<char32_t> &c, cursor_pos pos, int direction);

    [[nodiscard]] unsigned get_cursor_pos(unsigned pixels_x, unsigned line_idx);

    [[nodiscard]] unsigned get_pixel_pos(cursor_pos cursor);

    [[nodiscard]] inline size_t length() const { return m_length; };

    [[nodiscard]] inline const std::vector<line *> &lines() const { return m_lines; };
};


#endif //BLINK_DOCUMENT_HPP
