#include "texture.hpp"


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

void Texture::tex_unit(Shader shader, const char* uniform, GLuint unit) {
    GLuint tex_uni = glGetUniformLocation(shader.ID, uniform);
    shader.activate_shader();
    glUniform1i(tex_uni, unit);
}

void Texture::bind() { 
    glBindTexture(type, ID);
}

void Texture::unbind() {
    glBindTexture(type, 0);
}

void Texture::delete_texture() {
    glDeleteTextures(1, &ID);
}

