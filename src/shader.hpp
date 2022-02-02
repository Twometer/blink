//
// Created by Twometer on 2 Feb 2022.
//

#ifndef BLINK_SHADER_HPP
#define BLINK_SHADER_HPP

#include <map>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class shader {
private:
    GLuint program_id;
    std::map<std::string, GLint> uniform_cache;

public:
    explicit shader(GLuint programId);

    void set(const std::string &uniform, const glm::mat4 &value);
    void set(const std::string &uniform, const glm::vec3 &value);
    void set(const std::string &uniform, const glm::vec4 &value);
    void set(const std::string &uniform, bool value);

    void bind() const;
    static void unbind();

private:
    GLint get_uniform_location(const std::string &uniform);

};


#endif //BLINK_SHADER_HPP
