#include "Renderer.hpp"
#include "Skybox.hpp"
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

void Renderer::init_skybox(const std::vector<std::string>& faces, std::shared_ptr<Shader> shader) {
    skybox_enabled = true;
    skybox_shader = shader;
    skybox_texture = std::make_shared<Texture>(faces, GL_TEXTURE0);

    skybox_mesh = std::make_shared<Mesh>();
    skybox_mesh->init_positions_only(skybox_vertices, sizeof(skybox_vertices));

    skybox_mesh->set_vertex_cnt(36);
    skybox_mesh->set_draw_mode(MeshDrawMode::Arrays);

}


void Renderer::render_skybox(const Camera& camera, int screen_width, int screen_height) {
    if (!skybox_enabled || !skybox_shader || !skybox_texture || !skybox_mesh) {
        return;
    }

    // Change depth function so skybox is drawn behind all other objects
    glDepthFunc(GL_LEQUAL);

    skybox_shader->activate_shader();

    // Remove camera translation from view matrix
    glm::mat4 view = glm::mat4(glm::mat3(camera.get_view_matrix()));
    glm::mat4 projection = camera.get_projection_matrix(screen_width, screen_height);

    skybox_shader->set_mat4("view", view);
    skybox_shader->set_mat4("projection", projection);
    skybox_texture->tex_unit(*skybox_shader, "skybox", 0); // Bind to texture unit 0
    skybox_texture->bind();

    skybox_mesh->draw(); // Only positions, no EBO

    // Reset depth function
    glDepthFunc(GL_LESS);
}

void Renderer::clear() {
    render_queue.clear();
}

void Renderer::set_light(const Light& new_light) {
    light = new_light;
}
