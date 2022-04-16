//
// Created by Twometer on 16 Apr 2022.
//

#ifndef BLINK_CURSOR_HPP
#define BLINK_CURSOR_HPP

struct cursor_pos {
    unsigned x = 0;
    unsigned y = 0;

    inline bool operator>(const cursor_pos &other) const {
        return y > other.y || (y == other.y && x > other.x);
    }

    inline bool operator>=(const cursor_pos &other) const {
        return y > other.y || (y == other.y && x >= other.x);
    }

    inline bool operator<(const cursor_pos &other) const {
        return y < other.y || (y == other.y && x < other.x);
    }

    inline bool operator<=(const cursor_pos &other) const {
        return y < other.y || (y == other.y && x <= other.y);
    }

    inline bool operator==(const cursor_pos &other) const {
        return x == other.x && y == other.y;
    }
};

#endif //BLINK_CURSOR_HPP
