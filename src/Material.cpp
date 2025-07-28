#include "Material.hpp"
#include <glm/glm.hpp>

Material::Material(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture)
    : shader(shader), texture(texture) {
    // Optional: any initialization or logging
}


void Material::bind() {
    shader->activate_shader();

    if (texture) {
        shader->set_bool("use_texture", true);
        texture->tex_unit(*shader, sampler_name.c_str(), 0); // Binds sampler2D to unit 0
        texture->bind(); // Binds the actual texture to GL_TEXTURE0
    } else {
        shader->set_bool("use_texture", false);
    }
}

