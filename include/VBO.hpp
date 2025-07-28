#pragma once

#include <glad/glad.h>
#include <stddef.h>
#include <vector>
#include <glm/glm.hpp>

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
