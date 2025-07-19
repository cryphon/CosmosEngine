#include "Renderer.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer() {}

Renderer::~Renderer() {} 

void Renderer::submit(const RenderCommand& render_cmd) {
    render_queue.push_back(render_cmd);
}

void Renderer::render_all(const Camera& camera, int screen_width, int screen_height) {
    // View and projection matrices
    glm::mat4 view = camera.get_view_matrix();
    glm::mat4 projection = camera.get_projection_matrix(screen_width, screen_height);

    // Light properties (temporary static light)
    glm::vec3 light_pos(2.0f, 2.0f, 2.0f);
    glm::vec3 light_color(1.0f, 1.0f, 1.0f);
    glm::vec3 view_pos = camera.get_position();

    for (const auto& cmd : render_queue) {
        auto shader = cmd.material->shader;

        shader->activate_shader();

        // Set per-frame uniforms
        shader->set_mat4("view", view);
        shader->set_mat4("projection", projection);
        shader->set_vec3("lightPos", light.position);
        shader->set_vec3("lightColor", light.color);
        shader->set_vec3("viewPos", view_pos);

        // Set per-object uniforms
        shader->set_mat4("model", cmd.transform);

        cmd.material->bind(); // binds texture and such
        cmd.mesh->draw();
    }

    render_queue.clear(); // empty for next frame
}

void Renderer::clear() {
    render_queue.clear();
}

void Renderer::set_light(const Light& new_light) {
    light = new_light;
}
