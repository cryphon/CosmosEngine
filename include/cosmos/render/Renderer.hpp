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
#include <cosmos/scene/Light.hpp>
#include <cosmos/render/gfx/GLStateCache.hpp>

// ==
// Forward Declare
// ==
namespace cosmos::scene { class Camera; }
namespace cosmos::assets { class SkyBox; }
namespace cosmos::render { class VBO; class EBO; class RenderCommand; class Shader; class Texture; }
namespace cosmos::ui { class UI; }
namespace cosmos::core { class Profiler; }

namespace cosmos::render {


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

    GLuint time_q[2] = {0, 0};
    int tq = 0;
};
}

