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
Renderer::Renderer() {
    init_per_view_ubo_();    
}

Renderer::~Renderer() { } 

void Renderer::submit(const render::RenderCommand& render_cmd) {
    render_queue.push_back(render_cmd);
}

void Renderer::render_all(const scene::Camera& camera, int screen_width, int screen_height) {

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);



    UniformContext ctx;
    // View and projection matrices
    ctx.view = camera.get_view_matrix();
    ctx.view_pos = camera.get_position();
    ctx.projection = camera.get_projection_matrix();
    ctx.light_pos = light.position;
    ctx.light_color = light.color;


    // set ui controlled ctx vars
    if(ui) {
        ctx.reflectivity = ui->reflectivity_slider;
        ctx.alpha = ui->alpha_slider;
    };

    update_per_view_ubo_(ctx);

    for (const auto& cmd : render_queue) {

        // --- Apply pipeline state for this draw (once per pass)
        state_cache_.apply(cmd.state);


        auto& material = *cmd.material;
        auto shader = material.shader;
        shader->activate_shader();

        ctx.model = cmd.transform.model_matrix;

        if (material.bind_uniforms) {
            material.bind_uniforms(*shader, ctx);
        }

        material.bind(); // binds textures, etc.
        cmd.mesh->draw(material.vertex_layout());

            }
    render_queue.clear(); // empty for next frame
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

    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);


    skybox_material->shader->activate_shader();
    if (skybox_material->bind_uniforms) {
        UniformContext dummy{};
        dummy.model = glm::mat4(1.0f); // if your preset needs model
        skybox_material->bind_uniforms(*skybox_material->shader, dummy);
    }

    skybox_material->bind();
    skybox_mesh->draw(skybox_material->vertex_layout());

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}

void Renderer::render_grid(const scene::Camera& camera, int screen_width, int screen_height, float size, float step) {
    if (!grid_enabled) return;

    grid_shader->activate_shader();
    grid_shader->set_vec3("gridColor", glm::vec3(0.4f)); // gray

    grid_vao.bind();
    glDrawArrays(GL_LINES, 0, grid_vert_cnt);
    grid_vao.unbind();
}


void Renderer::init_per_view_ubo_() {
    glGenBuffers(1, &per_view_ubo_);
    glBindBuffer(GL_UNIFORM_BUFFER, per_view_ubo_);

    // Size must match the std140 layout (see struct below).
    // We'll allocate once and update with glBufferSubData per frame.
    const GLsizeiptr per_view_size =
        sizeof(glm::mat4) * 2 +          // view, projection
        sizeof(glm::vec4) * 4;           // viewPos, lightPos, lightColor, params

    glBufferData(GL_UNIFORM_BUFFER, per_view_size, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, per_view_ubo_); // binding = 0 matches GLSL
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


void Renderer::update_per_view_ubo_(const UniformContext& ctx) {
    PerViewStd140 data{};
    data.view       = ctx.view;
    data.projection = ctx.projection;
    data.view_pos   = glm::vec4(ctx.view_pos, 1.0f);
    data.light_pos  = glm::vec4(ctx.light_pos, 1.0f);
    data.light_color= glm::vec4(ctx.light_color, 1.0f);
    data.params     = glm::vec4(ctx.reflectivity, ctx.alpha, 0.0f, 0.0f);

    glBindBuffer(GL_UNIFORM_BUFFER, per_view_ubo_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PerViewStd140), &data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}







void Renderer::clear() {
    render_queue.clear();
}

void Renderer::set_light(const scene::Light& new_light) {
    light = new_light;
}

}
