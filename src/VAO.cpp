#include "VAO.hpp"

void VAO::link_attr(VBO& vbo, GLuint layout, GLuint num_components, GLenum type, GLsizeiptr stride, void* offset) {
    vbo.bind();
    glVertexAttribPointer(layout, num_components, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
    vbo.unbind();
}

void VAO::create() {
    glGenVertexArrays(1, &ID);
}

void VAO::bind() const {
    glBindVertexArray(ID);
}

void VAO::unbind() const {
    glBindVertexArray(0);
}

void VAO::delete_vao() {
    glDeleteVertexArrays(1, &ID);
}

// VAO.cpp
void VAO::link_layout(VBO& vbo, const VertexLayout& layout) {
    bind();          // make sure this VAO is current
    vbo.bind();      // make sure glVertexAttribPointer uses THIS VBO

    for (const auto& attr : layout.attributes) {
        glEnableVertexAttribArray(attr.index);
        glVertexAttribPointer(
            attr.index,
            attr.size,
            attr.type,
            GL_FALSE,
            layout.stride,
            reinterpret_cast<void*>(attr.offset)
        );
    }

    vbo.unbind();
    // leave the VAO bound if you’ll continue configuring; else:
    // unbind();
}

