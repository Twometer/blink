//
// Created by Twometer on 15 Apr 2022.
//

#include "mesh.hpp"

mesh::mesh(GLfloat *vertex_data, GLsizeiptr length) {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, length, vertex_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindVertexArray(0);

    m_num_tris = (GLsizei)(length / 4);
}

void mesh::bind() const {
    glBindVertexArray(m_vao);
}

void mesh::draw() const {
    glDrawArrays(GL_TRIANGLES, 0, m_num_tris);
}
