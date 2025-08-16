// ==
// Standard Library
// ==

// ==
// Third Party
// ==


// ==
// Cosmos
// ==
#include <cosmos/render/VBO.hpp>

namespace cosmos::render {

VBO::VBO(const float* vertices, size_t v_size) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, v_size, vertices, GL_STATIC_DRAW);

    size_t float_count = v_size / sizeof(float);
    vertices_data_.assign(vertices, vertices + float_count);
}

VBO::~VBO() { reset(); }

void VBO::reset() {
    if(ID) {
        glDeleteBuffers(1, &ID); ID = 0;
        vertices_data_.clear();
        vertices_data_.shrink_to_fit();
    }
}

std::vector<glm::vec3> VBO::get_vertices(int stride) const {
    std::vector<glm::vec3> result;
    for (size_t i = 0; i + 2 < vertices_data_.size(); i += stride) {
        result.emplace_back(vertices_data_[i], vertices_data_[i + 1], vertices_data_[i + 2]);
    }
    return result;
}

void VBO::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}
