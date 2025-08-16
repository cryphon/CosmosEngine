#pragma once
// ==
// Standard Library
// ==
#include <memory>
#include <functional>

// ==
// Third Party
// ==


// ==
// Cosmos
// ==
#include <cosmos/render/Shader.hpp>
#include <cosmos/render/VertexLayouts.hpp>

// ==
// Forward Declare
// ==
namespace cosmos::render { class UniformContext; class Texture; }

namespace cosmos::render {
class Material {
public:
    Material(std::shared_ptr<Shader> shader,
             std::shared_ptr<Texture> texture = nullptr,
             VertexLayoutView layout = VertexLayouts::BasicOBJ);
    virtual void bind();
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Texture> texture;
    std::string sampler_name = "uTexture";
    std::function<void(Shader& shader, const UniformContext& ctx)> bind_uniforms;
    VertexLayoutView vertex_layout() const {
        return { layout_.stride,
                 layout_.attributes.data(),
                 layout_.attributes.size() };
    }    

    void set_vertex_layout(VertexLayoutView layout) {
        layout_.stride = layout.stride;
        layout_.attributes.assign(layout.begin(), layout.end());
    }

    void apply_uniforms(const UniformContext& ctx) {
        shader->activate_shader();
        if(bind_uniforms) bind_uniforms(*shader, ctx);
    }

private:
    VertexLayoutDesc layout_;   // points to a static layout singleton
};
}
