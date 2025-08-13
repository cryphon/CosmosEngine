#include "MainScene.hpp"

#include <cosmos/render/Renderer.hpp>
#include <cosmos/render/Shader.hpp>
#include <cosmos/render/Mesh.hpp>
#include <cosmos/render/Transform.hpp>
#include <cosmos/render/Material.hpp>
#include <cosmos/render/UniformContext.hpp>
#include <cosmos/render/UniformPresets.hpp>
#include <cosmos/render/RenderCommand.hpp>
#include <cosmos/render/VertexLayouts.hpp>
#include <cosmos/assets/ShaderLibrary.hpp>
#include <cosmos/assets/SkyboxUtils.hpp>
#include <cosmos/scene/SceneObject.hpp>
#include <cosmos/assets/ObjLoader.hpp>


void MainScene::initialize(){ 

    cosmos::assets::ShaderLibrary::load("basic", "shaders/basic.vert", "shaders/basic.frag");
    cosmos::assets::ShaderLibrary::load("xyzmap", "shaders/xyzmap.vert", "shaders/xyzmap.frag");
    cosmos::assets::ShaderLibrary::load("default", "shaders/default.vert", "shaders/default.frag");
    cosmos::assets::ShaderLibrary::load("highlight", "shaders/passthrough.vert", "shaders/highlight.frag");

    renderer->set_highlight_shader(cosmos::assets::ShaderLibrary::get("highlight"));


    // --- Shader binds ---
    auto default_bind = [](cosmos::render::Shader& shader, const cosmos::render::UniformContext& ctx) {
    shader.set_mat4("model", ctx.model);
    shader.set_mat4("view", ctx.view);
    shader.set_mat4("projection", ctx.projection);

    shader.set_vec3("lightPos", ctx.light_pos);
    shader.set_vec3("viewPos", ctx.view_pos);
    shader.set_vec3("lightColor", ctx.light_color);

    shader.set_bool("selected", false);       // change this if highlighting
    shader.set_bool("use_texture", true);     // set to false if no texture is used
    };

    auto default_material = std::make_shared<cosmos::render::Material>(cosmos::assets::ShaderLibrary::get("default"));
    default_material->bind_uniforms = default_bind;

    auto xyz_material = std::make_shared<cosmos::render::Material>(cosmos::assets::ShaderLibrary::get("xyzmap"));
    xyz_material->bind_uniforms = cosmos::render::UniformPresets::normal_debug_bind; 

    auto basic_material = std::make_shared<cosmos::render::Material>(cosmos::assets::ShaderLibrary::get("basic"));
    basic_material->bind_uniforms = cosmos::render::UniformPresets::basic_bind; 

    // --- Light Shader ---
    cosmos::scene::Light light1({1.0f, 10.0f, 5.0f}, {1.0f, 1.0f, 1.0f});
    renderer->set_light(light1);
    auto light_mesh = cosmos::assets::ObjLoader::load("models/Sphere.obj");
    cosmos::render::Transform light_transform;
    light_transform.position = light1.position;
    light_transform.cache_trigger = true;
    light_transform.update_matrices();
    objects.emplace_back("light1", light_mesh, xyz_material, light_transform);

    
    auto mesh = cosmos::assets::ObjLoader::load("models/Human.obj");            
    cosmos::render::Transform transform;
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
            renderer->set_light(cosmos::scene::Light{
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

    if (skybox) {
        renderer->render_skybox(*camera, 1000, 1000, skybox);
    }

    renderer->clear();
}

void MainScene::render_ui() {}

void MainScene::cleanup() { }
void MainScene::on_enter() {}
void MainScene::on_exit() {}
