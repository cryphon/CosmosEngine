#pragma once
// ==
// Standard Library
// ==
#include <vector>
#include <utility>

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
    GLuint ID{0};
    VBO(const float* vertices, size_t v_size);


    // --- Non copyable ---
    VBO(const VBO&) = delete;
    VBO& operator=(const VBO&) = delete;

    // --- Movable ---
    VBO(VBO&& other) noexcept
        : ID(std::exchange(other.ID, 0))
        , vertices_data_(std::move(other.vertices_data_)) {}

    VBO& operator=(VBO&& other) noexcept {
        if(this != &other) {
            reset();
            ID = std::exchange(other.ID, 0);
            vertices_data_ = std::move(other.vertices_data_);
        }
        return *this;
    }

    ~VBO();


    void bind();
    void unbind();

    void reset();

    std::vector<glm::vec3> get_vertices(int stride = 11) const;

private: 
    std::vector<float> vertices_data_;
};
}
