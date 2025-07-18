#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>
#include <stddef.h>

class EBO {
    public:
        GLuint ID;

        EBO(const unsigned int* indices, size_t size);
        void bind();
        void unbind();
        void delete_ebo();
};

#endif
