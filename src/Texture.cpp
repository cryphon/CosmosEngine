#include "Texture.hpp"
#include "Logger.hpp"


Texture::Texture(const char* image, GLenum tex_type, GLenum slot, GLenum format, GLenum pixel_type) {
    type = tex_type;
    int width_img, height_img, num_cols;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(image, &width_img, &height_img, &num_cols, 0);

    glGenTextures(1, &ID);
    glActiveTexture(slot);
    glBindTexture(tex_type, ID);

	glTexParameteri(tex_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(tex_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(tex_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(tex_type, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(tex_type, 0, GL_RGBA, width_img, height_img, 0, format, pixel_type, bytes);
    glGenerateMipmap(tex_type);

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

void Texture::bind() { 
    glActiveTexture;
    glBindTexture(type, ID);
}

void Texture::unbind() {
    glBindTexture(type, 0);
}

void Texture::delete_texture() {
    glDeleteTextures(1, &ID);
}

