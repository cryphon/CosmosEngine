#pragma once
#include <glad/glad.h>
#include <stb/stb_image.h>
#include <vector>
#include "Shader.hpp"

class Texture {
    public:
        GLuint ID;
        GLenum type;
        GLenum unit;

        Texture(GLuint id, GLenum target = GL_TEXTURE_CUBE_MAP, GLenum slot = GL_TEXTURE0)
        : ID(id), type(target), unit(slot) {}
        Texture(const std::string& image, GLenum tex_type, GLenum slot, GLenum format, GLenum pixel_type);
        Texture(const std::vector<std::string>& faces, GLenum slot);
        void tex_unit(Shader& shader, const char* uniform, GLuint unit);
        void bind(GLenum slot = GL_TEXTURE0);
        void unbind();
        void delete_texture();
};
