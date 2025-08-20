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
#include <cosmos/render/Shader.hpp>

namespace cosmos::render {
Material::Material(std::shared_ptr<Shader> shader,
                   std::shared_ptr<Texture> texture,
                   VertexLayoutView layout)
    : shader(std::move(shader)),
      texture(std::move(texture)),
      layout_{ layout.stride, { layout.begin(), layout.end() } }  // copy
{}

void Material::bind() {
    shader->activate_shader();
}
}
