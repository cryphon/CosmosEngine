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


class Renderer {
public:
    Renderer();
    ~Renderer();

    void submit(const RenderCommand& render_cmd);
    void render_all(const Camera& camera, int screen_width, int screen_height);
    void init_skybox(const std::vector<std::string>& faces, std::shared_ptr<Shader> shader);
    void render_skybox(const Camera& camera, int screen_width, int screen_height);
    void init_grid(std::shared_ptr<Shader> shader, float size = 10.0f, float step = 1.0f);
    void render_grid(const Camera& camera, int screen_width, int screen_height, float size = 10.0f, float step = 1.0f);
    void clear();

    void set_light(const Light& light);
    Light get_light() { return light; }
    std::shared_ptr<Material> get_skybox_material() { return skybox_material; }
    bool skybox_enabled = false;
    bool grid_enabled = true;

    void set_selected_object(int id) { selected_object_id = id; }
    void set_highlight_shader(std::shared_ptr<Shader> shader) { highlight_shader = shader; }

private:
    std::vector<RenderCommand> render_queue;
    Light light = Light({2.0f, 2.0f, 2.0f}, {1.0f, 1.0f, 1.0f});

    // --- Sky Box ---
    std::shared_ptr<Mesh> skybox_mesh;
    std::shared_ptr<Material> skybox_material;
    // --- Grid Mesh ---
    std::unique_ptr<VBO> grid_vbo;
    VAO grid_vao;
    std::shared_ptr<Shader> grid_shader;
    int grid_vert_cnt;


    int selected_object_id = -1;

    std::shared_ptr<Shader> highlight_shader;
};

