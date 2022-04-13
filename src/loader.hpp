//
// Created by Twometer on 2 Feb 2022.
//

#ifndef BLINK_LOADER_HPP
#define BLINK_LOADER_HPP

#include <string>
#include <glad/glad.h>
#include "shader.hpp"

struct file_contents {
    uint8_t *data;
    size_t size;
};

class loader {
private:
    static void check_shader(const std::string &name, GLuint shader);

public:
    static shader *load_shader(const char *vert, const char *frag);

    static file_contents load_file(const std::string &path);
};


#endif //BLINK_LOADER_HPP
