//
// Created by Twometer on 15 Apr 2022.
//

#ifndef BLINK_SELECTION_HPP
#define BLINK_SELECTION_HPP

class selection {
private:
    unsigned x0;
    unsigned y0;
    unsigned x1;
    unsigned y1;

public:
    bool contains(unsigned x, unsigned y);
};


#endif //BLINK_SELECTION_HPP
