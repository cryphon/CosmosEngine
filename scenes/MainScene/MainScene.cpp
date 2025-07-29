#include "MainScene.hpp"
#include "PerspectiveCamera.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"
#include "SceneObject.hpp"
#include "ObjLoader.hpp"
#include "Transform.hpp"
#include "ShaderLibrary.hpp"
#include "UniformContext.hpp"
#include "UniformPresets.hpp"
#include <GLFW/glfw3.h>

void MainScene::initialize(){ 

    ShaderLibrary::load("basic", "shaders/basic.vert", "shaders/basic.frag");
    ShaderLibrary::load("xyzmap", "shaders/xyzmap.vert", "shaders/xyzmap.frag");
    ShaderLibrary::load("default", "shaders/default.vert", "shaders/default.frag");
    ShaderLibrary::load("highlight", "shaders/passthrough.vert", "shaders/highlight.frag");


    renderer->set_highlight_shader(ShaderLibrary::get("highlight"));

    // --- Shader binds ---
    auto default_bind = [](Shader& shader, const UniformContext& ctx) {
    shader.set_mat4("model", ctx.model);
    shader.set_mat4("view", ctx.view);
    shader.set_mat4("projection", ctx.projection);

    shader.set_vec3("lightPos", ctx.light_pos);
    shader.set_vec3("viewPos", ctx.view_pos);
    shader.set_vec3("lightColor", ctx.light_color);

    shader.set_bool("selected", false);       // change this if highlighting
    shader.set_bool("use_texture", true);     // set to false if no texture is used
    };

    auto default_material = std::make_shared<Material>(ShaderLibrary::get("default"));
    default_material->bind_uniforms = default_bind;

    auto xyz_material = std::make_shared<Material>(ShaderLibrary::get("xyzmap"));
    xyz_material->bind_uniforms = UniformPresets::normal_debug_bind; 

    auto basic_material = std::make_shared<Material>(ShaderLibrary::get("basic"));
    basic_material->bind_uniforms = UniformPresets::basic_bind; 



    // --- Light Shader ---
    Light light1({1.0f, 10.0f, 5.0f}, {1.0f, 1.0f, 1.0f});
    renderer->set_light(light1);
    auto light_mesh = ObjLoader::load("models/Sphere.obj");
    Transform light_transform;
    light_transform.position = light1.position;
    light_transform.cache_trigger = true;
    light_transform.update_matrices();
    objects.emplace_back("light1", light_mesh, xyz_material, light_transform);


    // 
    
    auto mesh = ObjLoader::load("models/Human.obj");            
    Transform transform;
    transform.position = {0.0f, 0.0f, 0.0f};
    transform.cache_trigger = true;
    transform.update_matrices();
    objects.emplace_back("human", mesh, default_material, transform);

}

void MainScene::update(float dt) { 
    rotation += rotation_speed * dt;

    // --- Crude lighting update fix
    for (auto& obj : objects) {
    if (obj.name == "light1") {
        renderer->set_light(Light{
            obj.transform.position,
            renderer->get_light().color // keep previous color
        });
    }
}


}
void MainScene::render() { 
    for (auto& obj : objects) {
        if (obj.transform.cache_trigger) {
            obj.transform.update_matrices(); // updates model matrix
        }
        // Convert SceneObject into RenderCommand
        renderer->submit({ obj.mesh, obj.material, obj.transform, obj.get_id()});
    }
    renderer->render_all(*camera, 1000, 1000);
    renderer->clear();
}

void MainScene::render_ui() {}

void MainScene::cleanup() { }
void MainScene::on_enter() {}
void MainScene::on_exit() {}
