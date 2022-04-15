//
// Created by Twometer on 15 Apr 2022.
//

#include "document.hpp"

unsigned document::insert(const std::string &data, unsigned offset) {
    unsigned num_inserted = data.length();
    m_buffer.add_text(data, offset);
    m_dirty = true;
    m_length += num_inserted;
    return num_inserted;
}

unsigned document::insert(char c, unsigned offset) {
    m_buffer.add_char(c, offset);
    m_dirty = true;
    m_length++;
    return 1;
}

void document::remove(unsigned i, unsigned len) {
    m_buffer.remove_text(i, len);
    m_dirty = true;
    m_length -= len;
}

void document::shape(font &font) {
    if (m_dirty) {
        m_shaped = m_buffer.shape(font, 0, 0);
        m_dirty = false;
    }
}

unsigned document::get_cursor_pos(unsigned text_pos) {
    if (text_pos <= 0) return 0;

    for (auto &glyph: m_shaped) {
        if (glyph.cluster_idx == text_pos)
            return glyph.pos_x;
    }

    auto &last_glyph = m_shaped[m_shaped.size() - 1];
    return last_glyph.pos_x + last_glyph.advance + 1;
}

unsigned document::find_one_of(std::set<char> c, unsigned int offset, int direction) {
    unsigned idx = offset + direction;
    while (idx >= 0 && idx < m_length) {
        if (c.contains(m_buffer.at(idx))) return idx;
        idx += direction;
    }
    return direction < 0 ? 0 : m_length;
}
