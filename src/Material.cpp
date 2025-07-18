#include "Material.hpp"
#include <glm/glm.hpp>

Material::Material(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture)
    : shader(shader), texture(texture) {
    // Optional: any initialization or logging
}


void Material::bind() {
    shader->activate_shader();
    // here we can set other uniforms like use_texture
    // --- use_texture ---
    shader->activate_shader();
    if (texture) {
        shader->set_bool("use_texture", true);
        texture->bind();
    } else {
        shader->set_bool("use_texture", false);
    }
}
