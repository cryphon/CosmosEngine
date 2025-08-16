// ==
// Standard Library
// ==

// ==
// Third Party
// ==

#include <stb/stb_image.h>

// ==
// Cosmos
// ==
#include <cosmos/render/TextureHDR.hpp>
#include <cosmos/core/Logger.hpp>

namespace cosmos::render {
GLuint load_hdr_texture(const std::string& path) {
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    float* data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);

    if (!data) {
        LOG_ERROR("Failed to load HDR image: " + path);
        return 0;
    }

    GLuint hdr_texture;
    glGenTextures(1, &hdr_texture);
    glBindTexture(GL_TEXTURE_2D, hdr_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return hdr_texture;
}

}

