// ==
// Standard Library
// ==

// ==
// Third Party
// ==
#include <glm/glm.hpp>

// ==
// Cosmos
// ==
#include <cosmos/render/Material.hpp>
#include <cosmos/render/Texture.hpp>

namespace cosmos::render {
Material::Material(std::shared_ptr<Shader> shader,  std::shared_ptr<Texture> texture, const VertexLayout& layout)
    : shader(shader), texture(texture), layout_(&layout) {
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
}
