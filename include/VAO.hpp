#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>
#include "VBO.hpp"

class VAO {
    public:
        GLuint ID;
        
        VAO() = default;

        void create();
        void link_attr(VBO& vbo, GLuint layout, GLuint num_components, GLenum type, GLsizeiptr stride, void* offset);
        void bind() const;
        void unbind();
        void delete_vao();
};

#endif
