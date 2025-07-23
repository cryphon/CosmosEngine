#include "MainScene.hpp"
#include "PerspectiveCamera.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"
#include "SceneObject.hpp"
#include "ObjLoader.hpp"
#include <GLFW/glfw3.h>

void MainScene::initialize(){ 
    auto shader = std::make_shared<Shader>("shaders/default.vert", "shaders/default.frag");

    Light light1({1.0f, 5.0f, 2.0f}, {1.0f, 1.0f, 1.0f});
    renderer->set_light(light1);

    auto mesh = ObjLoader::load("models/Human.obj");            
    auto texture = std::make_shared<Texture>("pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    auto material = std::make_shared<Material>(shader, texture);
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    objects.push_back({mesh, material, transform});

}

void MainScene::update(float dt) { 
    rotation += rotation_speed * dt;
}
void MainScene::render() { 
    for (auto& obj : objects) {
        // Convert SceneObject into RenderCommand
        renderer->submit({ obj.mesh, obj.material, obj.transform });
    }
    renderer->render_all(*camera, 1000, 1000);
    renderer->clear();
}

void MainScene::render_ui() {}

void MainScene::cleanup() { }
void MainScene::on_enter() {}
void MainScene::on_exit() {}
