// AppBootstrap.cpp

#include <cosmos/assets/MaterialLibrary.hpp>
#include <cosmos/core/AppBootstrap.hpp>
#include <cosmos/scene/Camera.hpp>
#include <cosmos/core/Window.hpp>
#include <cosmos/scene/SceneManager.hpp>
#include <cosmos/assets/ShaderLibrary.hpp>
#include <cosmos/assets/SkyboxManager.hpp>
#include <cosmos/assets/SkyboxUtils.hpp>
#include <cosmos/input/InputManager.hpp>
#include <cosmos/render/Renderer.hpp>
#include <cosmos/ui/Ui.hpp>
#include "MainScene.hpp"
#include "SecondScene.hpp"

using cosmos::core::AppBootstrap;


void AppBootstrap::bootstrap(cosmos::core::Engine& engine,
                             const EngineServices&  svc,
                             AppContext&            app)
{
    // 1) Create camera + controls
    app.camera   = std::make_shared<scene::Camera>();

    app.camera->set_position(glm::vec3(0.0f, 0.0f, 10.0f));
    app.camera->set_aspect_ratio(svc.window.get_aspect_ratio());
    app.camera->update_view();
    app.camera->update_projection();
    app.controls = std::make_shared<scene::OrbitalCameraControls>(svc.window.get_glfw_ref(), app.camera, glm::vec3(0.0f));
    app.camera_adapter = std::make_unique<scene::CameraInputAdapter>(app.controls.get(), *app.camera);
    svc.window.get_inputmanager()->add_listener(app.camera_adapter.get());

    svc.window.set_resize_callback([camera = app.camera, &window = svc.window](int width, int height) {
        camera->set_aspect_ratio(window.get_aspect_ratio());
        camera->update_projection();
    });

    svc.renderer.init_grid();

    // 2) Load resources (moved from Engine::init)
    app.skybox_manager = std::make_shared<assets::SkyBoxManager>();
    auto studio_skybox = init_hdri_skybox(app.skybox_manager, "studio", "textures/skybox/brown_photostudio.hdr");
    auto loft_skybox = init_hdri_skybox(app.skybox_manager, "loft", "textures/skybox/newport_loft.hdr");
    assets::ShaderLibrary::load("PBR", "shaders/pbr.vert", "shaders/pbr.frag");

    // shader has to be loaded before materials
    assets::MaterialLibrary::load_from_path("roofing", "assets/materials/roofing");
    assets::MaterialLibrary::load_from_path("bricks", "assets/materials/bricks");
    assets::MaterialLibrary::load_from_path("marble", "assets/materials/marble");
    assets::MaterialLibrary::load_from_path("gold", "assets/materials/gold");
    assets::MaterialLibrary::load_from_path("rock", "assets/materials/rock");

    // 3) Register scenes (capture what they need from svc/app)
    svc.scene_manager.register_factory("main", [r=&svc.renderer, cam=app.camera, studio_skybox](){
        auto scene = std::make_unique<MainScene>(r, cam);
        scene->set_skybox(studio_skybox);
        return scene;  
    });
    svc.scene_manager.register_factory("debug", [r=&svc.renderer, cam=app.camera, loft_skybox](){
        auto scene = std::make_unique<SecondScene>(r, cam);
        scene->set_skybox(loft_skybox);
        return scene; 
    });

    // 4) UI initialization 
    // 4) UI initialization
    UiContext ui_ctx{
        svc.window,
        svc.renderer,
        svc.scene_manager,
        app.camera,
        app.controls,
        app.camera_adapter,
        app.skybox_manager
    };
    svc.ui.initialize(ui_ctx);

    // 5) Choose initial scene
    svc.scene_manager.set_scene("main");
}

