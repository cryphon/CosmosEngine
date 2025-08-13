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
        Material(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture = nullptr, const VertexLayout& layout = VertexLayouts::BasicOBJ);
        virtual void bind();
        std::shared_ptr<Shader> shader;
        std::shared_ptr<Texture> texture;
        std::string sampler_name = "uTexture";
        std::function<void(Shader& shader, const UniformContext& ctx)> bind_uniforms;
        virtual const VertexLayout& vertex_layout() const { return *layout_; }

        void set_vertex_layout(const VertexLayout& layout) { layout_ = &layout; }
        
        void apply_uniforms(const UniformContext& ctx) {
            shader->activate_shader();
            if(bind_uniforms) bind_uniforms(*shader, ctx);
        }

    private:
    const VertexLayout* layout_;   // points to a static layout singleton
};
}
