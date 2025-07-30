#include "SecondScene.hpp"
#include "PerspectiveCamera.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"
#include "Material.hpp"
#include <GLFW/glfw3.h>
#include "ShaderLibrary.hpp"
#include "UniformContext.hpp"
#include "ObjLoader.hpp"

void SecondScene::initialize(){ 
    ShaderLibrary::load("reflect", "shaders/reflect.vert", "shaders/reflect.frag");

    auto reflect_shader = ShaderLibrary::get("reflect");

    auto reflect_material = std::make_shared<Material>(reflect_shader);
    reflect_material->sampler_name = "skybox";
    reflect_material->bind_uniforms = [](Shader& shader, const UniformContext& ctx) {
        shader.set_mat4("model", ctx.model);
        shader.set_mat4("view", ctx.view);
        shader.set_mat4("projection", ctx.projection);
        shader.set_vec3("viewPos", ctx.view_pos);
        shader.set_float("reflectivity", ctx.reflectivity);
        shader.set_float("alpha", ctx.alpha);
    };
    reflect_material->texture = renderer->get_skybox_material()->texture;


    auto sphere = Mesh::create_uv_sphere(64, 32, 1.0f);

    Transform reflective_transform;
    reflective_transform.position = { 0.0f, 0.0f, 0.0f };
    reflective_transform.cache_trigger = true;
    reflective_transform.update_matrices();

    objects.emplace_back("reflective", std::move(sphere), reflect_material, reflective_transform);


}

void SecondScene::update(float dt) { 
    rotation += rotation_speed * dt;
}
void SecondScene::render() { 
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

void SecondScene::render_ui() {}

void SecondScene::cleanup() { }
void SecondScene::on_enter() {}
void SecondScene::on_exit() {}
