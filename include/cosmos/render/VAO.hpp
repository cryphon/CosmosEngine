#pragma once
// ==
// Standard Library
// ==

// ==
// Third Party
// ==
#include <glad/glad.h>

// ==
// Cosmos
// ==
#include <cosmos/render/VertexLayout.hpp>

// ==
// Forward Declare
// ==
namespace cosmos::render { class VBO; }

namespace cosmos::render {

class VAO {
public:
    GLuint ID;

    VAO() = default;

    // --- Non copyable ---
    VAO(const VAO&) = delete;
    VAO& operator=(const VAO&) = delete;

    // --- Movable ---
    VAO(VAO&& other) noexcept : ID(std::exchange(other.ID, 0)) {}
    VAO& operator=(VAO&& other) noexcept { if (this != &other) { reset(); ID = std::exchange(other.ID, 0);} return *this; }
    
    ~VAO();

    void create();
    void link_attr(VBO& vbo, GLuint layout, GLuint num_components, GLenum type, GLsizeiptr stride, void* offset);
    void link_layout(VBO& vbo, const VertexLayoutView& layout);
    void bind() const;
    void unbind() const;

    void reset();
};
}
