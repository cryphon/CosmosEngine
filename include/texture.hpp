#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "shader.hpp"

class Texture {
    public:
        GLuint ID;
        GLenum type;
        Texture(const char* image, GLenum tex_type, GLenum slot, GLenum format, GLenum pixel_type);
        void tex_unit(Shader shader, const char* uniform, GLuint unit);
        void bind();
        void unbind();
        void delete_texture();
};

#endif
