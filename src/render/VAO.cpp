// ==
// Standard Library
// ==

// ==
// Third Party
// ==


// ==
// Cosmos
// ==
#include <cosmos/render/VAO.hpp>
#include <cosmos/render/VBO.hpp>
#include <cosmos/render/VertexLayout.hpp>

namespace cosmos::render {

VAO::~VAO() { reset(); }

void VAO::reset() {
    if(ID) { glDeleteVertexArrays(1, &ID); ID = 0; }
}

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

// VAO.cpp
void VAO::link_layout(VBO& vbo, const VertexLayoutView& layout) {
    bind();          // make sure this VAO is current
    vbo.bind();      // make sure glVertexAttribPointer uses THIS VBO

    for (std::size_t i = 0; i < layout.count; ++i) {
        const auto& a = layout.attributes[i];
        glVertexAttribPointer(a.index, a.size, a.type, GL_FALSE, layout.stride,
                              reinterpret_cast<const void*>(a.offset));
        glEnableVertexAttribArray(a.index);
    }
    vbo.unbind();
}

}
