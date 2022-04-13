//
// Created by Twometer on 13 Apr 2022.
//

#include <stdexcept>
#include "atlas.hpp"

atlas::atlas(int size, int min_row_size) : m_size(size), m_min_row_size(min_row_size) {
    glGenTextures(1, &m_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, size, size, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
}

void atlas::insert(int codepoint, int w, int h, uint8_t *data) {
    auto &row = find_free_row(w, h);
    int x = row.offset;
    int y = row.y_pos;
    row.offset += w;

    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RED, GL_UNSIGNED_BYTE, data);
    auto size = (float) m_size;
    m_sprites[codepoint] = {(float) x / size, (float) y / size, (float) w / size, (float) h / size};
}

atlas_row &atlas::find_free_row(int w, int h) {
    int next_row_y = 0;
    for (auto &row: m_rows) {
        next_row_y = row.y_pos + row.height;
        if (row.height <= h && m_size - row.offset <= w) {
            return row;
        }
    }

    int new_row_size = std::max(h, m_min_row_size);

    if (next_row_y + new_row_size > m_size) {
        throw std::runtime_error("Texture atlas is out of space");
    }

    return this->m_rows.emplace_back(next_row_y, new_row_size, 0);
}

