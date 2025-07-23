#include "MainScene.hpp"
#include "PerspectiveCamera.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"
#include "SceneObject.hpp"
#include "ObjLoader.hpp"
#include "Transform.hpp"
#include <GLFW/glfw3.h>

void MainScene::initialize(){ 
    auto shader = std::make_shared<Shader>("shaders/default.vert", "shaders/default.frag");

    Light light1({1.0f, 10.0f, 5.0f}, {1.0f, 1.0f, 1.0f});
    renderer->set_light(light1);
    auto light_mesh = ObjLoader::load("models/Sphere.obj");
    auto light_mat = std::make_shared<Material>(shader);

    Transform light_transform;
    light_transform.position = light1.position;
    light_transform.update_matrices();
    objects.push_back(SceneObject({"light1", light_mesh, light_mat, light_transform}));


    auto mesh = ObjLoader::load("models/Human.obj");            
    auto material = std::make_shared<Material>(shader);
    Transform transform;
    transform.position = {0.0f, 0.0f, 0.0f};
    transform.update_matrices();
    objects.push_back({"human", mesh, material, transform});

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
