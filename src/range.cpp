//
// Created by Tim Buchner on 15.04.22.
//

#include "range.hpp"

bool range::contains(cursor_pos pos) const {
    return pos >= begin() && pos < end();
}

void range::collapse_begin() {
    set_to(begin());
}

void range::collapse_end() {
    set_to(end());
}

void range::set_to(cursor_pos pos) {
    m_pos1 = pos;
    m_pos2 = pos;
}

void range::reset() {
    set_to({0, 0});
}

void range::limit_y(unsigned max_y) {
    if (m_pos1.y > max_y) m_pos1.y = max_y;
    if (m_pos2.y > max_y) m_pos2.y = max_y;
}

void range::limit_x(unsigned int max_x) {
    if (m_pos1.x > max_x) m_pos1.x = max_x;
    if (m_pos2.x > max_x) m_pos2.x = max_x;
}
