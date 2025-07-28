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
        texture->bind();
        if (!sampler_name.empty()) {
            texture->tex_unit(*shader, sampler_name.c_str(), 0); // default unit 0
        }
    } else {
        shader->set_bool("use_texture", false);
    }
}

