#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "Camera.hpp"
#include "RenderCommand.hpp"
#include "Light.hpp"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void submit(const RenderCommand& render_cmd);
    void render_all(const Camera& camera, int screen_width, int screen_height);
    void clear();

    void set_light(const Light& light);

private:
    std::vector<RenderCommand> render_queue;
    Light light = Light({2.0f, 2.0f, 2.0f}, {1.0f, 1.0f, 1.0f});
};

