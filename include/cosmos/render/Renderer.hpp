#pragma once
// ==
// Standard Library
// ==
#include <memory>
#include <vector>

// ==
// Third Party
// ==
#include <glm/glm.hpp>

// ==
// Cosmos
// ==
#include <cosmos/render/VAO.hpp>
#include <cosmos/render/UniformContext.hpp>
#include <cosmos/scene/Light.hpp>
#include <cosmos/render/gfx/GLStateCache.hpp>
#include <cosmos/assets/ResourceManager.hpp>

// ==
// Forward Declare
// ==
namespace cosmos::scene { class Camera; }
namespace cosmos::assets { class SkyBox; }
namespace cosmos::render { class VBO; class EBO; class RenderCommand; class Shader; class Texture; }
namespace cosmos::ui { class UI; }
namespace cosmos::core { class Profiler; }

namespace cosmos::render {


// C++ pack structure matching the GLSL block (UBO)
struct PerViewStd140 {
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec4 view_pos;    // xyz used
    glm::vec4 light_pos;   // xyz used
    glm::vec4 light_color; // rgb used
    glm::vec4 params;      // x=reflectivity, y=alpha
};



class Renderer {
public:
    Renderer();
    ~Renderer();

    void submit(const render::RenderCommand& render_cmd);
    void render_all(const scene::Camera& camera, int screen_width, int screen_height);
    void render_skybox(const scene::Camera& camera, int screen_width, int screen_height, std::shared_ptr<assets::SkyBox> skybox);
    void init_grid(float size = 10.0f, float step = 1.0f);
    void render_grid(const scene::Camera& camera, int screen_width, int screen_height, float size = 10.0f, float step = 1.0f);
    void set_ui(std::shared_ptr<ui::UI> ui_ptr) { ui = ui_ptr; }
    void clear();
    void set_light(const scene::Light& light);
    scene::Light get_light() { return light; }
    bool skybox_enabled = true;
    bool grid_enabled = true;

    void set_selected_object(int id) { selected_object_id = id; }
    void set_highlight_shader(std::shared_ptr<Shader> shader) { highlight_shader = shader; }

    void set_profiler(cosmos::core::Profiler* p) { profiler_ = p; }
    void set_resources(cosmos::assets::ResourceManager* r) { resources_ = r; }
    cosmos::assets::ResourceManager& get_resources() { return *resources_; }
    
private:
    std::vector<RenderCommand> render_queue;
    gfx::GLStateCache state_cache_;
    std::shared_ptr<ui::UI> ui = nullptr;
    scene::Light light = scene::Light({2.0f, 2.0f, 2.0f}, {1.0f, 1.0f, 1.0f});

    // --- Grid Mesh ---
    std::unique_ptr<VBO> grid_vbo;
    VAO grid_vao;
    std::shared_ptr<Shader> grid_shader;
    int grid_vert_cnt;


    int selected_object_id = -1;

    std::shared_ptr<Shader> highlight_shader;

    cosmos::core::Profiler* profiler_ = nullptr;
    cosmos::assets::ResourceManager* resources_ = nullptr;

    GLuint time_q[2] = {0, 0};
    int tq = 0;


    GLuint per_view_ubo_ = 0;
    void init_per_view_ubo_();
    void update_per_view_ubo_(const UniformContext& ctx);
};
}

