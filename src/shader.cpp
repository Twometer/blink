//
// Created by Twometer on 2 Feb 2022.
//

#include "shader.hpp"

shader::shader(GLuint program_id)
        : program_id(program_id) {
}

GLint shader::get_uniform_location(const std::string &uniform) {
    if (uniform_cache.find(uniform) == uniform_cache.end()) {
        auto loc = glGetUniformLocation(program_id, uniform.c_str());
        uniform_cache[uniform] = loc;
        return loc;
    } else {
        return uniform_cache[uniform];
    }
}

void shader::set(const std::string &uniform, const glm::mat4 &value) {
    glUniformMatrix4fv(get_uniform_location(uniform), 1, false, &value[0][0]);
}

void shader::set(const std::string &uniform, const glm::vec3 &value) {
    glUniform3f(get_uniform_location(uniform), value.x, value.y, value.z);
}

void shader::set(const std::string &uniform, const glm::vec4 &value) {
    glUniform4f(get_uniform_location(uniform), value.x, value.y, value.z, value.w);
}

void shader::bind() const {
    glUseProgram(program_id);
}

void shader::unbind() {
    glUseProgram(0);
}

void shader::set(const std::string &uniform, bool value) {
    glUniform1i(get_uniform_location(uniform), value ? 1 : 0);
}
