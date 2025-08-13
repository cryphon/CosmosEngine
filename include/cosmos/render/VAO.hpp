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

        void create();
        void link_attr(VBO& vbo, GLuint layout, GLuint num_components, GLenum type, GLsizeiptr stride, void* offset);
        void link_layout(VBO& vbo, const VertexLayoutView& layout);
        void bind() const;
        void unbind() const;
        void delete_vao();
};
}
