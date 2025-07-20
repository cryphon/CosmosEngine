#include "SecondScene.hpp"
#include "PerspectiveCamera.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"
#include <GLFW/glfw3.h>

void SecondScene::initialize(){ 
    static float vertices[] = {
        // Position          Normal          Color        Tex Coords
        // Base
        -0.5f, 0.0f, -0.5f,   0, -1, 0,      1, 0, 0,     0, 0,   // 0
        0.5f, 0.0f, -0.5f,   0, -1, 0,      0, 1, 0,     1, 0,   // 1
        0.5f, 0.0f,  0.5f,   0, -1, 0,      0, 0, 1,     1, 1,   // 2
        -0.5f, 0.0f,  0.5f,   0, -1, 0,      1, 1, 0,     0, 1,   // 3

        // Top point
        0.0f, 0.8f,  0.0f,   0, 1, 0,       1, 0, 1,     0.5, 0.5 // 4
    };


    static unsigned int indices[] = {
        // Base (two triangles)
        0, 1, 2,
        2, 3, 0,

        // Sides (each a triangle from base to tip)
        0, 1, 4,  // front right
        1, 2, 4,  // back right
        2, 3, 4,  // back left
        3, 0, 4   // front left
    };

    quad_mesh = std::make_shared<Mesh>();
    quad_mesh->init(vertices, sizeof(vertices), indices, sizeof(indices));

    Light light1({1.0f, 5.0f, 2.0f}, {1.0f, 0.9f, 0.7f});
    renderer->set_light(light1);

    auto shader = std::make_shared<Shader>("shaders/default.vert", "shaders/default.frag");
    auto texture = std::make_shared<Texture>("pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    quad_material = std::make_shared<Material>(shader, texture);
}

void SecondScene::update(float dt) { 
    rotation += rotation_speed * dt;
}
void SecondScene::render() { 
    if (camera && renderer && quad_mesh && quad_material) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(scale));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
        renderer->submit({ quad_mesh, quad_material, model });
    }

    renderer->render_all(*camera, 1000, 1000, -1);
    renderer->clear();       
}

void SecondScene::render_ui() {
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

void SecondScene::cleanup() { }
void SecondScene::on_enter() {}
void SecondScene::on_exit() {}
