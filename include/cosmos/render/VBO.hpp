#pragma once
// ==
// Standard Library
// ==
#include <vector>

// ==
// Third Party
// ==
#include <glad/glad.h>
#include <glm/glm.hpp>

// ==
// Cosmos
// ==

namespace cosmos::render {

class VBO {
    public:
        GLuint ID;
        VBO(const float* vertices, size_t v_size);

        void bind();
        void unbind();
        void delete_vbo();

        std::vector<glm::vec3> get_vertices(int stride = 11) const;

    private: 
        std::vector<float> vertices_data;
};
}
