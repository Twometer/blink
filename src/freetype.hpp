//
// Created by Twometer on 13 Apr 2022.
//

#ifndef BLINK_FREETYPE_HPP
#define BLINK_FREETYPE_HPP


#include "freetype/freetype.h"

class freetype {
private:
    freetype() = default;

    static FT_Library ft;

public:

    static FT_Library get();
};


#endif //BLINK_FREETYPE_HPP
