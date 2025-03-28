#include "VAO.hpp"

VAO::VAO() {
    glGenVertexArrays(1, &ID);
}

void VAO::link_attr(VBO& vbo, GLuint layout, GLuint num_components, GLenum type, GLsizeiptr stride, void* offset) {
    vbo.bind();
    glVertexAttribPointer(layout, num_components, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
    vbo.unbind();
}

void VAO::bind() {
    glBindVertexArray(ID);
}

void VAO::unbind() {
    glBindVertexArray(0);
}

void VAO::delete_vao() {
    glDeleteVertexArrays(1, &ID);
}
