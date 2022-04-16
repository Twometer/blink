//
// Created by Twometer on 15 Apr 2022.
//

#ifndef BLINK_RANGE_HPP
#define BLINK_RANGE_HPP

#include "cursor.hpp"

class range {
private:
    cursor_pos m_pos1{};
    cursor_pos m_pos2{};

public:
    [[nodiscard]] bool contains(cursor_pos pos) const;

    void collapse_begin();

    void collapse_end();

    void set_to(cursor_pos pos);

    void reset();

    [[nodiscard]] inline const cursor_pos &begin() const { return m_pos1 < m_pos2 ? m_pos1 : m_pos2; };

    [[nodiscard]] inline const cursor_pos &end() const { return m_pos1 < m_pos2 ? m_pos2 : m_pos1; };

    [[nodiscard]] inline bool empty() const { return m_pos1 == m_pos2; };

    inline void set_pos1(cursor_pos pos) { m_pos1 = pos; };

    inline void set_pos2(cursor_pos pos) { m_pos2 = pos; };

    void limit_y(unsigned max_y);
};


#endif //BLINK_RANGE_HPP
