//
// Created by Tim Buchner on 15.04.22.
//

#include "selection.hpp"

bool selection::contains(cursor_pos pos) const {
    return pos.x >= m_begin.x && pos.y >= m_begin.y &&
           pos.x <= m_end.x && pos.y <= m_end.y;
}

void selection::collapse_begin() {
    m_end = m_begin;
}

void selection::collapse_end() {
    m_begin = m_end;
}
