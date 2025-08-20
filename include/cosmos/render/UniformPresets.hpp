#pragma once
// ==
// Standard Library
// ==

// ==
// Third Party
// ==


// ==
// Cosmos
// ==
#include <cosmos/render/Shader.hpp>
#include <cosmos/render/UniformContext.hpp>


namespace cosmos::render::UniformPresets {
inline auto basic_bind = [](Shader& shader, const UniformContext& ctx) {
    shader.set_mat4("model", ctx.model);
};

inline auto full_lighting_bind = [](Shader& shader, const UniformContext& ctx) {
    shader.set_mat4("model", ctx.model);
};

inline auto normal_debug_bind = [](Shader& shader, const UniformContext& ctx) {
    shader.set_mat4("model", ctx.model);
};

inline auto skybox_bind = [](Shader& shader, const UniformContext& ctx) {
};

inline auto phong_bind = [](Shader& shader, const UniformContext& ctx) {
    shader.set_mat4("model", ctx.model);
    shader.set_vec3("uPhongDiffuse", glm::vec3(1.0f, 0.5f, 1.0f));
    shader.set_vec3("uPhongSpecular", glm::vec3(1.0f)); // white highlights
    shader.set_float("uPhongShininess", 32.0f);
};

}
