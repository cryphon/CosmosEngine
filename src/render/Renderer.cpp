// ==
// Standard Library
// ==

// ==
// Third Party
// ==
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

// ==
// Cosmos
// ==
#include <cosmos/render/Renderer.hpp>
#include <cosmos/render/Mesh.hpp>
#include <cosmos/render/Material.hpp>
#include <cosmos/render/UniformContext.hpp>
#include <cosmos/render/UniformPresets.hpp>
#include <cosmos/render/gfx/RenderCommand.hpp>
#include <cosmos/assets/Skybox.hpp>
#include <cosmos/scene/Camera.hpp>
#include <cosmos/ui/Ui.hpp>

namespace cosmos::render {
Renderer::Renderer() { }

Renderer::~Renderer() { } 

void Renderer::submit(const render::RenderCommand& render_cmd) {
    render_queue.push_back(render_cmd);
}

// Renderer.cpp  (replace your current render_all with this)
void Renderer::render_all(const scene::Camera& camera, int screen_w, int screen_h) {
    // == Build the per-frame base context (unchanged from your style)
    UniformContext base;
    base.view        = camera.get_view_matrix();
    base.view_pos    = camera.get_position();
    base.projection  = camera.get_projection_matrix();
    base.light_pos   = light.position;
    base.light_color = light.color;

    if (ui) { // keep whatever UI fields you already expose
        base.reflectivity = ui->reflectivity_slider;
        base.alpha        = ui->alpha_slider;
    }

    // == Partition render_queue into opaque / transparent and compute depth
    std::vector<const RenderCommand*> opaque;
    std::vector<const RenderCommand*> transparent;
    opaque.reserve(render_queue.size());
    transparent.reserve(render_queue.size());

    const glm::vec3 cam_pos = base.view_pos;
    for (auto& cmd : render_queue) {
        // depth: distance from camera to model translation
        const glm::vec3 pos = glm::vec3(cmd.transform.model_matrix[3]);
        const float d = glm::length(pos - cam_pos);
        // store depth on the command (if mutable) or keep it local:
        const_cast<RenderCommand&>(cmd).depth = d;

        (cmd.transparent ? transparent : opaque).push_back(&cmd);
    }

    // == Sort opaque to minimize state changes: shader -> material -> mesh
    auto by_pipeline = [](const RenderCommand* a, const RenderCommand* b){
        if (a->material->shader.get() != b->material->shader.get())
            return a->material->shader.get() < b->material->shader.get();
        if (a->material != b->material)
            return a->material < b->material;
        return a->mesh < b->mesh;
    };
    std::sort(opaque.begin(), opaque.end(), by_pipeline);

    // == Sort transparent back-to-front (largest depth first)
    std::sort(transparent.begin(), transparent.end(),
              [](const RenderCommand* a, const RenderCommand* b){
                  return a->depth > b->depth;
              });

    // == Draw helper (applies state cache, binds, draws) --------------------
    auto draw_cmd = [&](const RenderCommand& cmd, const gfx::RenderState& pass_override){
        // Start from the command's own state; override pass-specific bits.
        gfx::RenderState s = cmd.state;
        s.blending    = pass_override.blending;
        s.depth_write = pass_override.depth_write;
        s.blend       = pass_override.blend;
        state_cache_.apply(s);                      

        auto& material = *cmd.material;
        auto shader = material.shader;
        shader->activate_shader();

        UniformContext ctx = base;
        ctx.model = cmd.transform.model_matrix;

        if (material.bind_uniforms) material.bind_uniforms(*shader, ctx);
        material.bind();
        cmd.mesh->draw(material.vertex_layout());
    };

    // == OPAQUE PASS: depth write ON, blending OFF --------------------------
    {
        gfx::RenderState pass{};
        pass.depth_write = true;
        pass.blending    = false;
        pass.blend       = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA}; // not used, for consistency
        for (auto* p : opaque) draw_cmd(*p, pass);
    }

    // == TRANSPARENT PASS: depth write OFF, blending ON ---------------------
    {
        gfx::RenderState pass{};
        pass.depth_write = false;                    // read depth, don't write
        pass.blending    = true;
        pass.blend       = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
        for (auto* p : transparent) draw_cmd(*p, pass);
    }


    render_queue.clear();
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


    
void Renderer::render_skybox(const scene::Camera& camera, int screen_width, int screen_height, std::shared_ptr<assets::SkyBox> skybox) {
    auto skybox_material = skybox->get_material();
    auto skybox_mesh = skybox->get_mesh();
    
    if (!skybox_enabled || !skybox_material || !skybox_mesh ) {
        return;
    }

    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    UniformContext ctx;
    ctx.view = camera.get_view_matrix();
    ctx.projection = camera.get_projection_matrix();

    skybox_material->shader->activate_shader();
    if (skybox_material->bind_uniforms)
        skybox_material->bind_uniforms(*skybox_material->shader, ctx);

    skybox_material->bind();
    skybox_mesh->draw(skybox_material->vertex_layout());

    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
}

void Renderer::render_grid(const scene::Camera& camera, int screen_width, int screen_height, float size, float step) {
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

void Renderer::set_light(const scene::Light& new_light) {
    light = new_light;
}

}
