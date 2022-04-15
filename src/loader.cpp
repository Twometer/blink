//
// Created by Twometer on 2 Feb 2022.
//

#include <iostream>
#include "loader.hpp"

void loader::check_shader(const std::string &name, GLuint shader) {
    GLint result = 0;
    GLint logLength = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

    if (logLength) {
        char *data = static_cast<char *>(calloc(logLength + 1, 1));
        glGetShaderInfoLog(shader, logLength, nullptr, data);
        std::cout << "** Error in '" << name << "' **" << std::endl << data << std::endl;
        free(data);
    } else {
        std::cout << "info: Shader " << name << " compiled successfully" << std::endl;
    }

}

shader *loader::load_shader(const char *vert, const char *frag) {
    auto program = glCreateProgram();

    auto vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vert, nullptr);
    glCompileShader(vertex);
    check_shader("vert", vertex);

    auto fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &frag, nullptr);
    glCompileShader(fragment);
    check_shader("frag", fragment);

    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return new shader(program);

}

file_contents loader::load_file(const std::string &path) {
    FILE *fd = fopen(path.c_str(), "rb");

    fseek(fd, 0, SEEK_END);
    size_t file_size = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    auto *data = new uint8_t[file_size];
    fread(data, 1, file_size, fd);

    fclose(fd);

    return {data, file_size};
}
