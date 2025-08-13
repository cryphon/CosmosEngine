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
    vertices_data.assign(vertices, vertices + float_count);
}

std::vector<glm::vec3> VBO::get_vertices(int stride) const {
    std::vector<glm::vec3> result;
    for (size_t i = 0; i + 2 < vertices_data.size(); i += stride) {
        result.emplace_back(vertices_data[i], vertices_data[i + 1], vertices_data[i + 2]);
    }
    return result;
}

void VBO::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::delete_vbo() {
    glDeleteBuffers(1, &ID);
}

}
