// ==
// Standard Library
// ==

// ==
// Third Party
// ==


// ==
// Cosmos
// ==
#include <cosmos/render/EBO.hpp>

namespace cosmos::render {
EBO::EBO(const unsigned int* indices, size_t i_size) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, i_size, indices, GL_STATIC_DRAW);

    size_t index_count = i_size / sizeof(unsigned int);
    index_data_.assign(indices, indices + index_count);
}
EBO::~EBO() { reset(); }

void EBO::reset() {
    if(ID) { glDeleteBuffers(1, &ID); ID = 0; }
}

void EBO::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}
