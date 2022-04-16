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
