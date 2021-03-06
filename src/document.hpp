//
// Created by Twometer on 15 Apr 2022.
//

#ifndef BLINK_DOCUMENT_HPP
#define BLINK_DOCUMENT_HPP

#include <set>
#include "text_buffer.hpp"
#include "cursor.hpp"
#include "range.hpp"

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

    void remove_range(range range);

    void insert_line(unsigned pos_y);

    void insert_split_line(cursor_pos pos);

    void erase_line(unsigned pos_y);

    void shape(font &font);

    void ensure_lines();

    std::u32string to_string(std::optional<range> range = {});

    std::string to_utf8_string(std::optional<range> range = {});

    [[nodiscard]] unsigned find_one_of(const std::set<char32_t> &c, cursor_pos pos, int direction) const;

    [[nodiscard]] unsigned get_cursor_pos(unsigned pixels_x, unsigned line_idx) const;

    [[nodiscard]] unsigned get_pixel_pos(cursor_pos cursor) const;

    [[nodiscard]] inline size_t length() const { return m_length; };

    [[nodiscard]] inline const std::vector<line *> &lines() const { return m_lines; };

    [[nodiscard]] inline size_t num_lines() const { return m_lines.size(); };
};


#endif //BLINK_DOCUMENT_HPP
