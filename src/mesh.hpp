//
// Created by Twometer on 15 Apr 2022.
//

#ifndef BLINK_MESH_HPP
#define BLINK_MESH_HPP

#include <glad/glad.h>

class mesh {
private:
    GLuint m_vao = 0;
    GLuint m_vbo = 0;

public:
    mesh(GLfloat *vertex_data, GLsizeiptr length);

    void bind() const;
};


#endif //BLINK_MESH_HPP
