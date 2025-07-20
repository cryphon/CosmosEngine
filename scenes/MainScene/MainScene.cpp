#include "MainScene.hpp"
#include "PerspectiveCamera.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"
#include <GLFW/glfw3.h>

void MainScene::initialize(){ 
    static float vertices[] = {
        // pos              | normal         | color       | tex coords
        // Front face (+Z)
        -0.5f, -0.5f,  0.5f,  0, 0, 1,   1, 1, 1,   0, 0,
        0.5f, -0.5f,  0.5f,  0, 0, 1,   1, 1, 1,   1, 0,
        0.5f,  0.5f,  0.5f,  0, 0, 1,   1, 1, 1,   1, 1,
        -0.5f,  0.5f,  0.5f,  0, 0, 1,   1, 1, 1,   0, 1,

        // Back face (-Z)
        -0.5f, -0.5f, -0.5f,  0, 0, -1,  1, 1, 1,   1, 0,
        0.5f, -0.5f, -0.5f,  0, 0, -1,  1, 1, 1,   0, 0,
        0.5f,  0.5f, -0.5f,  0, 0, -1,  1, 1, 1,   0, 1,
        -0.5f,  0.5f, -0.5f,  0, 0, -1,  1, 1, 1,   1, 1,

        // Left face (-X)
        -0.5f, -0.5f, -0.5f, -1, 0, 0,   1, 1, 1,   0, 0,
        -0.5f, -0.5f,  0.5f, -1, 0, 0,   1, 1, 1,   1, 0,
        -0.5f,  0.5f,  0.5f, -1, 0, 0,   1, 1, 1,   1, 1,
        -0.5f,  0.5f, -0.5f, -1, 0, 0,   1, 1, 1,   0, 1,

        // Right face (+X)
        0.5f, -0.5f, -0.5f,  1, 0, 0,   1, 1, 1,   1, 0,
        0.5f, -0.5f,  0.5f,  1, 0, 0,   1, 1, 1,   0, 0,
        0.5f,  0.5f,  0.5f,  1, 0, 0,   1, 1, 1,   0, 1,
        0.5f,  0.5f, -0.5f,  1, 0, 0,   1, 1, 1,   1, 1,

        // Top face (+Y)
        -0.5f,  0.5f, -0.5f,  0, 1, 0,   1, 1, 1,   0, 1,
        0.5f,  0.5f, -0.5f,  0, 1, 0,   1, 1, 1,   1, 1,
        0.5f,  0.5f,  0.5f,  0, 1, 0,   1, 1, 1,   1, 0,
        -0.5f,  0.5f,  0.5f,  0, 1, 0,   1, 1, 1,   0, 0,

        // Bottom face (-Y)
        -0.5f, -0.5f, -0.5f,  0, -1, 0,  1, 1, 1,   0, 0,
        0.5f, -0.5f, -0.5f,  0, -1, 0,  1, 1, 1,   1, 0,
        0.5f, -0.5f,  0.5f,  0, -1, 0,  1, 1, 1,   1, 1,
        -0.5f, -0.5f,  0.5f,  0, -1, 0,  1, 1, 1,   0, 1,
    };

    static unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,        // front
        4, 5, 6, 6, 7, 4,        // back
        8, 9,10,10,11, 8,        // left
        12,13,14,14,15,12,        // right
        16,17,18,18,19,16,        // top
        20,21,22,22,23,20         // bottom
    };
    quad_mesh = std::make_shared<Mesh>();
    quad_mesh->init(vertices, sizeof(vertices), indices, sizeof(indices));

    Light light1({1.0f, 5.0f, 2.0f}, {1.0f, 0.9f, 0.7f});
    renderer->set_light(light1);

    auto shader = std::make_shared<Shader>("shaders/default.vert", "shaders/default.frag");
    auto texture = std::make_shared<Texture>("pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    quad_material = std::make_shared<Material>(shader, texture);
}

void MainScene::update(float dt) { 
    rotation += rotation_speed * dt;
}
void MainScene::render() { 
    if (camera && renderer && quad_mesh && quad_material) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(scale));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
        renderer->submit({ quad_mesh, quad_material, model });
    }

    renderer->render_all(*camera, 1000, 1000, -1);
    renderer->clear();       
}

void MainScene::render_ui() {
    if(ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Simulation Settings")) {
            ImGui::SliderFloat("Scale", &scale, 0.1f, 5.0f);
            ImGui::SliderFloat("Rotation Speed", &rotation_speed, -500.0f, 500.0f);

            if (ImGui::Button("Reset Cam")) {
                std::shared_ptr<PerspectiveCamera> perspCam = std::dynamic_pointer_cast<PerspectiveCamera>(camera);
                if (perspCam) {
                    perspCam->reset_camera();
                }
            }
            ImGui::EndMenu();
        }
    }
    ImGui::EndMainMenuBar();
}

void MainScene::cleanup() { }
void MainScene::on_enter() {}
void MainScene::on_exit() {}
