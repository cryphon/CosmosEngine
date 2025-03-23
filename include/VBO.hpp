#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>

class VBO {
    public:
        GLuint ID;
        VBO(GLfloat* vertices, GLsizeiptr size);

        void bind();
        void unbind();
        void delete_vbo();
};

#endif
