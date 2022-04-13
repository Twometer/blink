//
// Created by Twometer on 13 Apr 2022.
//

#include <stdexcept>
#include "freetype.hpp"

FT_Library freetype::ft = nullptr;

FT_Library freetype::get() {
    if (ft != nullptr || FT_Init_FreeType(&ft) == 0) {
        return ft;
    } else {
        throw std::runtime_error("Failed to initialize FreeType library!");
    }
}
