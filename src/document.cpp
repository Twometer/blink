//
// Created by Twometer on 15 Apr 2022.
//

#include "document.hpp"

unsigned document::insert(const std::u32string &data, unsigned pos_x, unsigned pos_y) {
    if (m_lines.empty()) insert_line(0);

    auto &line = m_lines[pos_y];

    unsigned num_inserted = data.length();
    line->buffer.add_text(data, pos_x);
    line->dirty = true;
    m_length += num_inserted;
    return num_inserted;
}

unsigned document::insert(char32_t c, unsigned pos_x, unsigned pos_y) {
    if (m_lines.empty()) insert_line(0);

    auto &line = m_lines[pos_y];

    line->buffer.add_char(c, pos_x);
    line->dirty = true;
    m_length++;
    return 1;
}

void document::remove(unsigned pos_x, unsigned pos_y, unsigned len) {
    auto &line = m_lines[pos_y];

    line->buffer.remove_text(pos_x, len);
    line->dirty = true;
    m_length -= len;
}

void document::insert_line(unsigned int pos_y) {
    m_lines.insert(m_lines.begin() + pos_y, new line());
}

void document::shape(font &font) {
    for (auto &line: m_lines) {
        if (line->dirty) {
            line->glyphs = line->buffer.shape(font);
            line->dirty = false;
        }
    }
}

unsigned document::get_char_pos(unsigned int cursor_pos_x, unsigned int line_idx) {
    if (cursor_pos_x == 0) return 0;
    auto &line = m_lines[line_idx];

    for (auto &glyph: line->glyphs) {
        if (glyph.pos_x + glyph.advance / 2 >= cursor_pos_x) {
            return glyph.cluster_idx;
        }
    }
    auto &last_glyph = line->glyphs[line->glyphs.size() - 1];
    return last_glyph.cluster_idx + 1;
}

unsigned document::get_cursor_pos(unsigned char_pos_x, unsigned char_pos_y) {
    if (char_pos_x == 0) return 0;

    auto &line = m_lines[char_pos_y];

    for (auto &glyph: line->glyphs) {
        if (glyph.cluster_idx == char_pos_x)
            return glyph.pos_x;
    }

    auto &last_glyph = line->glyphs[line->glyphs.size() - 1];
    return last_glyph.pos_x + last_glyph.advance + 1;
}

unsigned document::find_one_of(const std::set<char> &c, unsigned int pos_x, unsigned int pos_y, int direction) {
    auto &buffer = m_lines[pos_y]->buffer;
    unsigned idx = pos_x + direction;
    while (idx >= 0 && idx < m_length) {
        if (c.contains(buffer.at(idx))) return idx;
        idx += direction;
    }
    return direction < 0 ? 0 : m_length;
}

document::document() {
    insert_line(0);
}

document::~document() {
    for (auto line: lines()) {
        delete line;
    }
}

void document::insert_split_line(unsigned int pos_x, unsigned int pos_y) {
    auto line = m_lines[pos_y];
    line->dirty = true;
    auto removed = line->buffer.remove_text_and_get(pos_x, line->buffer.size() - pos_x);
    insert_line(pos_y + 1);
    m_lines[pos_y + 1]->buffer.add_text(removed, 0);
}

void document::erase_line(unsigned int pos_y) {
    auto erased = m_lines[pos_y];
    m_lines.erase(m_lines.begin() + pos_y);
    delete erased;

}


