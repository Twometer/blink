//
// Created by Twometer on 15 Apr 2022.
//

#include "document.hpp"

unsigned document::insert(const std::string &data, unsigned pos_x, unsigned pos_y) {
    auto &line = m_lines[pos_y];

    unsigned num_inserted = data.length();
    line.buffer.add_text(data, pos_x);
    line.dirty = true;
    m_length += num_inserted;
    return num_inserted;
}

unsigned document::insert(char c, unsigned pos_x, unsigned pos_y) {
    auto &line = m_lines[pos_y];

    line.buffer.add_char(c, pos_x);
    line.dirty = true;
    m_length++;
    return 1;
}

void document::remove(unsigned pos_x, unsigned pos_y, unsigned len) {
    auto &line = m_lines[pos_y];

    line.buffer.remove_text(pos_x, len);
    line.dirty = true;
    m_length -= len;
}

void document::insert_line(unsigned int pos_y) {
    m_lines.insert(m_lines.begin() + pos_y, {});
}

void document::shape(font &font) {
    for (auto &line: m_lines) {
        if (line.dirty) {
            line.glyphs = line.buffer.shape(font);
            line.dirty = false;
        }
    }
}

unsigned document::get_cursor_pos(unsigned char_pos_x, unsigned char_pos_y) {
    if (char_pos_x == 0) return 0;

    auto &line = m_lines[char_pos_y];

    for (auto &glyph: line.glyphs) {
        if (glyph.cluster_idx == char_pos_x)
            return glyph.pos_x;
    }

    auto &last_glyph = line.glyphs[line.glyphs.size() - 1];
    return last_glyph.pos_x + last_glyph.advance + 1;
}

unsigned document::find_one_of(std::set<char> c, unsigned int pos_x, unsigned int pos_y, int direction) {
    auto &buffer = m_lines[pos_y].buffer;
    unsigned idx = pos_x + direction;
    while (idx >= 0 && idx < m_length) {
        if (c.contains(buffer.at(idx))) return idx;
        idx += direction;
    }
    return direction < 0 ? 0 : m_length;
}
