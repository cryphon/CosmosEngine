#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>
#include <stddef.h>

class VBO {
    public:
        GLuint ID;
        VBO(const float* vertices, size_t v_size);

        void bind();
        void unbind();
        void delete_vbo();
};

#endif
