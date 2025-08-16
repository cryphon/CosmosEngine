#pragma once
// ==
// Standard Library
// ==
#include <stddef.h>
#include <vector>
#include <utility>

// ==
// Third Party
// ==
#include <glad/glad.h>

// ==
// Cosmos
// ==

namespace cosmos::render {
class EBO {
public:
    GLuint ID{0};

    EBO() = default; 
    EBO(const unsigned int* indices, size_t size);

    // --- Non copyable ---
    EBO(const EBO&) = delete;
    EBO& operator=(const EBO&) = delete;
    
    // --- Movable ---
    EBO(EBO&& other) noexcept
        : ID(std::exchange(other.ID, 0)) {}
    EBO& operator=(EBO&& other) noexcept { if (this != &other) { reset(); ID = std::exchange(other.ID, 0);} return *this; }
    ~EBO();

    void bind();
    void unbind();

    void reset();

    const std::vector<unsigned int>& get_indices() const { return index_data_; };

private:
    std::vector<unsigned int> index_data_;
};
}

