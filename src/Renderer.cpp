#include "Renderer.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer() {}

Renderer::~Renderer() {} 

void Renderer::submit(const RenderCommand& render_cmd) {
    render_queue.push_back(render_cmd);
}

void Renderer::render_all(const Camera& camera, int screen_width, int screen_height) {
    for (const auto& rend : render_queue) {
        rend.material->bind();

        // Upload model, view, proj matrices
        rend.material->shader->set_mat4("model", rend.transform);
        rend.material->shader->set_mat4("view", camera.get_view_matrix());
        rend.material->shader->set_mat4("proj", camera.get_projection_matrix(screen_width, screen_height));

        rend.mesh->draw();
    }
}

void Renderer::clear() {
    render_queue.clear();
}
