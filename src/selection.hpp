//
// Created by Twometer on 15 Apr 2022.
//

#ifndef BLINK_SELECTION_HPP
#define BLINK_SELECTION_HPP

#include "cursor.hpp"

class selection {
private:
    cursor_pos m_begin{};
    cursor_pos m_end{};

public:
    [[nodiscard]] bool contains(cursor_pos pos) const;

    void collapse_begin();

    void collapse_end();

    [[nodiscard]] inline const cursor_pos &begin() const { return m_begin; };

    [[nodiscard]] inline const cursor_pos &end() const { return m_end; };

    inline void set_begin(cursor_pos pos) { m_begin = pos; };

    inline void set_end(cursor_pos pos) { m_end = pos; };
};


#endif //BLINK_SELECTION_HPP
