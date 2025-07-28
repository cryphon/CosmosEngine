#pragma once
#include "Shader.hpp"
#include "Texture.hpp"
#include <memory>
#include <functional>

class UniformContext;


class Material {
    public:
        Material(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture = nullptr);
        void bind();
        std::shared_ptr<Shader> shader;
        std::shared_ptr<Texture> texture;
        std::string sampler_name = "uTexture";
        std::function<void(Shader& shader, const UniformContext& ctx)> bind_uniforms;
        
        void apply_uniforms(const UniformContext& ctx) {
            shader->activate_shader();
            if(bind_uniforms) bind_uniforms(*shader, ctx);
        }
};
