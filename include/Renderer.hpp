#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "RenderCommand.hpp"
#include "Light.hpp"

class Camera;
class UI;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void submit(const RenderCommand& render_cmd);
    void render_all(const Camera& camera, int screen_width, int screen_height);
    void init_skybox(const std::vector<std::string>& faces, std::shared_ptr<Shader> shader);
    void init_hdri_skybox(const std::string& hdr_path, std::shared_ptr<Shader> capture_shader, std::shared_ptr<Shader> render_shader);
    void render_skybox(const Camera& camera, int screen_width, int screen_height);
    void init_grid(std::shared_ptr<Shader> shader, float size = 10.0f, float step = 1.0f);
    void render_grid(const Camera& camera, int screen_width, int screen_height, float size = 10.0f, float step = 1.0f);
    void set_ui(std::shared_ptr<UI> ui_ptr) { ui = ui_ptr; }
    void clear();

    void set_light(const Light& light);
    Light get_light() { return light; }
    std::shared_ptr<Material> get_skybox_material() { return skybox_material; }
    bool skybox_enabled = false;
    bool grid_enabled = true;

    void set_selected_object(int id) { selected_object_id = id; }
    void set_highlight_shader(std::shared_ptr<Shader> shader) { highlight_shader = shader; }
    void set_default_camera(std::shared_ptr<Camera> cam) {
        default_camera = cam;
    }

    const Camera& get_camera_or_default(const Camera* cam_ptr) const {
        if (cam_ptr) return *cam_ptr;
        return *default_camera;
    }

private:
    std::vector<RenderCommand> render_queue;
    std::shared_ptr<UI> ui = nullptr;
    Light light = Light({2.0f, 2.0f, 2.0f}, {1.0f, 1.0f, 1.0f});

    // --- Sky Box ---
    std::shared_ptr<Mesh> skybox_mesh;
    std::shared_ptr<Material> skybox_material;
    // --- Grid Mesh ---
    std::unique_ptr<VBO> grid_vbo;
    VAO grid_vao;
    std::shared_ptr<Shader> grid_shader;
    int grid_vert_cnt;
    std::shared_ptr<Camera> default_camera;


    int selected_object_id = -1;

    std::shared_ptr<Shader> highlight_shader;
};

