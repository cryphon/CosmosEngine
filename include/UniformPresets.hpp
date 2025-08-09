#include "Shader.hpp"
#include "UniformContext.hpp"

namespace UniformPresets {
inline auto basic_bind = [](Shader& shader, const UniformContext& ctx) {
    shader.set_mat4("model", ctx.model);
    shader.set_mat4("view", ctx.view);
    shader.set_mat4("projection", ctx.projection);
};

inline auto full_lighting_bind = [](Shader& shader, const UniformContext& ctx) {
    shader.set_mat4("model", ctx.model);
    shader.set_mat4("view", ctx.view);
    shader.set_mat4("projection", ctx.projection);
    shader.set_vec3("lightPos", ctx.light_pos);
    shader.set_vec3("viewPos", ctx.view_pos);
    shader.set_vec3("lightColor", ctx.light_color);
};

inline auto normal_debug_bind = [](Shader& shader, const UniformContext& ctx) {
    shader.set_mat4("model", ctx.model);
    shader.set_mat4("view", ctx.view);
    shader.set_mat4("projection", ctx.projection);
};

inline auto skybox_bind = [](Shader& shader, const UniformContext& ctx) {
    glm::mat4 view_no_translation = glm::mat4(glm::mat3(ctx.view));
    shader.set_mat4("view", view_no_translation);
    shader.set_mat4("projection", ctx.projection);
};

inline auto pbr_bind = [](Shader& shader, const UniformContext& ctx) {
    shader.set_mat4("model", ctx.model);
    shader.set_mat4("view", ctx.view);
    shader.set_mat4("projection", ctx.projection);

    shader.set_vec3("lightPos", ctx.light_pos);
    shader.set_vec3("viewPos", ctx.view_pos);
    shader.set_vec3("lightColor", ctx.light_color);
};


}
