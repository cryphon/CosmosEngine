// ==
// Standard Library
// ==

// ==
// Third Party
// ==


// ==
// Cosmos
// ==
#include <cosmos/render/Texture.hpp>
#include <cosmos/render/Shader.hpp>
#include <cosmos/core/Logger.hpp>

namespace cosmos::render {

Texture::Texture(const std::string& image, GLenum tex_type, GLenum slot, GLenum format, GLenum pixel_type) {
    type = tex_type;
    int width_img, height_img, num_cols;
    stbi_set_flip_vertically_on_load(false);
    unsigned char* bytes = stbi_load(image.c_str(), &width_img, &height_img, &num_cols, 0);

    if (!bytes) {
        LOG_ERROR("Failed to load texture: " + std::string(image));
        return; // Don't continue
    }

    GLenum data_format, internal_format;

    switch (num_cols) {
        case 1:
            data_format = GL_RED;
            internal_format = GL_RED;
            break;
        case 3:
            data_format = GL_RGB;
            internal_format = GL_RGB;
            break;
        case 4:
            data_format = GL_RGBA;
            internal_format = GL_RGBA;
            break;
        default:
            LOG_ERROR("Unsupported channel count: " + std::to_string(num_cols));
            stbi_image_free(bytes);
            return;
    }

    glGenTextures(1, &ID);
    glActiveTexture(slot);
    glBindTexture(tex_type, ID);

    glTexParameteri(tex_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(tex_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(tex_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(tex_type, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(tex_type, 0, internal_format, width_img, height_img, 0, data_format, pixel_type, bytes);
    glGenerateMipmap(tex_type);

    LOG_INFO("Loaded texture: " + std::string(image));
    stbi_image_free(bytes);
    glBindTexture(tex_type, 0);
}



Texture::Texture(const std::vector<std::string>& faces, GLenum slot) {
    type = GL_TEXTURE_CUBE_MAP;
    glGenTextures(1, &ID);
    glActiveTexture(slot);
    glBindTexture(type, ID);

    
    stbi_set_flip_vertically_on_load(false);

    int width, height, n_channels;
    for(unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &n_channels, 0);
        if(data) {
            glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                    GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        } else {
            LOG_ERROR(std::string("Cubemap texture failed to load at path: ") + faces[i]);
            stbi_image_free(data);
        }
    }

    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(type, 0);

}

void Texture::tex_unit(Shader& shader, const char* uniform, GLuint unit) {
    GLuint tex_uni = glGetUniformLocation(shader.ID, uniform);
    shader.activate_shader();
    glUniform1i(tex_uni, unit);
}

void Texture::bind(GLenum slot) { 
    glActiveTexture(slot);
    glBindTexture(type, ID);
}

void Texture::unbind() {
    glBindTexture(type, 0);
}

void Texture::delete_texture() {
    glDeleteTextures(1, &ID);
}

}
