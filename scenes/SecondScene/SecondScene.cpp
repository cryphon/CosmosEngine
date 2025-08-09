#include "SecondScene.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"
#include "Material.hpp"
#include <GLFW/glfw3.h>
#include "ShaderLibrary.hpp"
#include "UniformContext.hpp"
#include "ObjLoader.hpp"
#include "PBRMaterial.hpp"
#include "MaterialLibrary.hpp"

void SecondScene::initialize(){ 

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


 
    auto pbr_material = MaterialLibrary::get("roofing");
    pbr_material->albedo = glm::vec3(1.0f, 0.8f, 0.6f);
    pbr_material->roughness = 0.5f;
    pbr_material->metallic = 0.0f;
        auto sphere2 = Mesh::create_uv_sphere(256, 128, 1.0f);

    Transform t0, t1, t2;
    t0.position = {0.0f, 0.0f, 0.0f };
    t1.position = { -1.5f, 0.0f, 0.0f };
    t2.position = { 1.5f, 0.0f, 0.0f };
    t1.cache_trigger = t2.cache_trigger = true;
    t1.update_matrices();
    t2.update_matrices();

    //objects.emplace_back("basic", std::move(sphere1), default_material, t1);
    objects.emplace_back("pbr",   std::move(sphere2), pbr_material, t0);

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
