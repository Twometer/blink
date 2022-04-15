//
// Created by Twometer on 15 Apr 2022.
//

#include "document.hpp"

void document::insert(const std::string &data, int offset) {
    m_buffer.add_text(data);
    m_dirty = true;
}

void document::render(font &font) {
    if (m_dirty) {
        m_shaped = m_buffer.shape(font, 0, 0);
        m_dirty = false;
    }
}
