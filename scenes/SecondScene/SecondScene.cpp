// ==
// Standard Library
// ==

// ==
// Third Party
// ==

// ==
// Cosmos
// ==
#include "SecondScene.hpp"
#include <cosmos/render/Shader.hpp>
#include <cosmos/render/Renderer.hpp>
#include <cosmos/render/Material.hpp>
#include <cosmos/render/Mesh.hpp>
#include <cosmos/render/Transform.hpp>
#include <cosmos/render/Texture.hpp>
#include <cosmos/render/PBRMaterial.hpp>
#include <cosmos/render/UniformContext.hpp>
#include <cosmos/render/RenderCommand.hpp>
#include <cosmos/assets/ShaderLibrary.hpp>
#include <cosmos/assets/MaterialLibrary.hpp>
#include <cosmos/assets/SkyboxUtils.hpp>
#include <cosmos/assets/ObjLoader.hpp>
#include <cosmos/scene/SceneObject.hpp>

void SecondScene::initialize(){ 

        auto pbr_material = cosmos::assets::MaterialLibrary::get("rock");
        auto sphere2 = cosmos::render::Mesh::create_uv_sphere(256, 128, 1.0f);

    cosmos::render::Transform t0, t1, t2;
    t0.position = {0.0f, 0.0f, 0.0f };
    t1.position = { -1.5f, 0.0f, 0.0f };
    t2.position = { 1.5f, 0.0f, 0.0f };
    t1.cache_trigger = t2.cache_trigger = true;
    t1.update_matrices();
    t2.update_matrices();

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

    if (skybox) {
        renderer->render_skybox(*camera, 1000, 1000, skybox);
    }

    renderer->clear();
}

void SecondScene::render_ui() {}

void SecondScene::cleanup() { }
void SecondScene::on_enter() {}
void SecondScene::on_exit() {}
