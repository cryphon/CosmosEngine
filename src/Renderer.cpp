#include "Renderer.hpp"
#include "Skybox.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "UniformContext.hpp"
#include "UniformPresets.hpp"
#include "Ui.hpp"

Renderer::Renderer() {}

Renderer::~Renderer() {} 

void Renderer::submit(const RenderCommand& render_cmd) {
    render_queue.push_back(render_cmd);
}

void Renderer::render_all(const Camera& camera, int screen_width, int screen_height) {
    UniformContext ctx;
    // View and projection matrices
    ctx.view = camera.get_view_matrix();
    ctx.view_pos = camera.get_position();
    ctx.projection = camera.get_projection_matrix();

    // Light properties (temporary static light)
    ctx.light_pos = light.position;
    ctx.light_color = light.color;


    // set ui controlled ctx vars
    if(ui) {
        ctx.reflectivity = ui->reflectivity_slider;
        ctx.alpha = ui->alpha_slider;
    };

    for (const auto& cmd : render_queue) {
        auto& material = *cmd.material;
        auto shader = material.shader;
        shader->activate_shader();

        ctx.model = cmd.transform.model_matrix;

        if (material.bind_uniforms) {
            material.bind_uniforms(*shader, ctx);
        }

        material.bind(); // binds textures, etc.
        cmd.mesh->draw();
    }

    // Draw highlight on top
    // Highlight pass (2nd draw)
    if (selected_object_id != -1) {
        for (const auto& cmd : render_queue) {
            if (cmd.object_id == selected_object_id) {
                highlight_shader->activate_shader();
                highlight_shader->set_mat4("model", cmd.transform.model_matrix);
                highlight_shader->set_mat4("view", camera.get_view_matrix());
                highlight_shader->set_mat4("projection", camera.get_projection_matrix());
                highlight_shader->set_vec3("color", glm::vec3(1.0f, 1.0f, 0.0f)); // Yellow outline

                // Optional wireframe
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                cmd.mesh->draw();
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

                break;
            }
        }
    }    render_queue.clear(); // empty for next frame
}

void Renderer::init_skybox(const std::vector<std::string>& faces, std::shared_ptr<Shader> shader) {
    skybox_material = std::make_shared<Material>(shader, std::make_shared<Texture>(faces, GL_TEXTURE0));
    skybox_material->sampler_name = "skybox";
    skybox_material->bind_uniforms = UniformPresets::skybox_bind; 

    skybox_mesh = std::make_shared<Mesh>();
    skybox_mesh->init_positions_only(skybox_vertices, sizeof(skybox_vertices));
    skybox_mesh->set_vertex_cnt(36);
    skybox_mesh->set_draw_mode(MeshDrawMode::Arrays);

}

void Renderer::init_grid(float size, float step){
    auto shader = std::make_shared<Shader>("shaders/grid.vert", "shaders/grid.frag");
    std::vector<float> grid_vertices;

    for (float i = -size; i <= size; i += step) {
        // lines parallel to X axis (vary Z)
        grid_vertices.insert(grid_vertices.end(), { -size, 0.0f, i, size, 0.0f, i });
        // lines parallel to Z axis (vary X)
        grid_vertices.insert(grid_vertices.end(), { i, 0.0f, -size, i, 0.0f, size });
    }

    grid_shader = shader;
    grid_vert_cnt = grid_vertices.size() / 3; // 3 floats per vertex

    grid_vao.create();
    grid_vao.bind();

    grid_vbo = std::make_unique<VBO>(grid_vertices.data(), grid_vertices.size() * sizeof(float));
    grid_vao.link_attr(*grid_vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

    grid_vao.unbind();
    grid_vbo->unbind();}
    


void Renderer::render_skybox(const Camera& camera, int screen_width, int screen_height) {
    if (!skybox_enabled || !skybox_material || !skybox_mesh) {
        return;
    }

    glDepthFunc(GL_LEQUAL);

    UniformContext ctx;
    ctx.view = camera.get_view_matrix();
    ctx.projection = camera.get_projection_matrix();

    skybox_material->shader->activate_shader();
    if (skybox_material->bind_uniforms)
        skybox_material->bind_uniforms(*skybox_material->shader, ctx);

    skybox_material->bind();
    skybox_mesh->draw();

    glDepthFunc(GL_LESS);
}

#include "TextureHDR.hpp"
#include "SkyboxUtils.hpp"
#include "SkyboxManager.hpp"

void Renderer::init_hdri_skybox(const std::string& hdr_path) {
    auto capture_shader = std::make_shared<Shader>("shaders/hdr_to_cubemap.vert", "shaders/hdr_to_cubemap.frag");
    auto render_shader = std::make_shared<Shader>("shaders/skybox.vert", "shaders/skybox.frag");
    auto skybox_manager = std::make_shared<SkyBoxManager>();

    // 1. Load HDR texture
    GLuint hdr_texture = load_hdr_texture(hdr_path);
    if (hdr_texture == 0) {
        std::cerr << "Failed to load HDR texture at path: " << hdr_path << std::endl;
        return;
    }

    // 2. Setup framebuffer and renderbuffer for capturing cubemap
    GLuint capture_fbo, capture_rbo;
    glGenFramebuffers(1, &capture_fbo);
    glGenRenderbuffers(1, &capture_rbo);

    // 3. Convert HDR texture to cubemap
    GLuint cubemap = equirectangular_to_cubemap(hdr_texture, capture_shader, capture_fbo, capture_rbo);

    // 4. Clean up FBOs
    glDeleteFramebuffers(1, &capture_fbo);
    glDeleteRenderbuffers(1, &capture_rbo);
    glDeleteTextures(1, &hdr_texture); // We don’t need the 2D HDR texture anymore

    // 5. Create skybox material
    skybox_material = std::make_shared<Material>(render_shader, std::make_shared<Texture>(cubemap, GL_TEXTURE_CUBE_MAP, GL_TEXTURE0));
    skybox_material->sampler_name = "skybox";
    skybox_material->bind_uniforms = UniformPresets::skybox_bind;

    // 6. Setup cube mesh
    skybox_mesh = std::make_shared<Mesh>();
    skybox_mesh->init_positions_only(skybox_vertices, sizeof(skybox_vertices));
    skybox_mesh->set_vertex_cnt(36);
    skybox_mesh->set_draw_mode(MeshDrawMode::Arrays);

    skybox_manager->register_factory("loft", [this]() {
        return std::make_unique<SkyBox>(skybox_mesh, skybox_material);
    });

    skybox_enabled = true;
}


void Renderer::render_grid(const Camera& camera, int screen_width, int screen_height, float size, float step) {
    if (!grid_enabled) return;

    grid_shader->activate_shader();
    grid_shader->set_mat4("view", camera.get_view_matrix());
    grid_shader->set_mat4("projection", camera.get_projection_matrix());
    grid_shader->set_vec3("gridColor", glm::vec3(0.4f)); // gray

    grid_vao.bind();
    glDrawArrays(GL_LINES, 0, grid_vert_cnt);
    grid_vao.unbind();
}

void Renderer::clear() {
    render_queue.clear();
}

void Renderer::set_light(const Light& new_light) {
    light = new_light;
}
