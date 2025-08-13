// ==
// Standard Library
// ==
#include <memory>

// ==
// Third Party
// ==
#include <glm/gtx/string_cast.hpp>
#include <imgui_impl_opengl3.h>

// ==
// Cosmos
// ==
#include <cosmos/core/Engine.hpp>
#include <cosmos/core/Window.hpp>
#include <cosmos/render/Renderer.hpp>
#include <cosmos/input/InputManager.hpp>
#include <cosmos/assets/SkyboxManager.hpp>
#include <cosmos/assets/SkyboxUtils.hpp>
#include <cosmos/assets/MaterialLibrary.hpp>
#include <cosmos/scene/Camera.hpp>
#include <cosmos/scene/SceneManager.hpp>
#include <cosmos/scene/Scene.hpp>
#include <cosmos/ui/Ui.hpp>
#include "MainScene.hpp"
#include "SecondScene.hpp"


#define SCREEN_WIDTH 1200.0f
#define SCREEN_HEIGHT 800.0f

namespace cosmos::core {
Engine::Engine() {}
Engine::~Engine() {
    glfwDestroyWindow(window_->get_glfw_ref());
    glfwTerminate();
}


bool Engine::init() {
    window_ = new Window(1200, 800, "Cosmos Engine", false, 4);
    window_->set_clear_color(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    //window_->enable_vsync();
    window_->activate();
    window_->init_ui();
    window_->init_inputmanager();
    auto ui = window_->get_ui();

    renderer_ = std::make_shared<render::Renderer>();
    scene_manager_ = std::make_shared<scene::SceneManager>();
    renderer_->set_ui(ui);


    camera_ = std::make_shared<scene::Camera>();
    camera_->set_position(glm::vec3(0.0f, 0.0f, 10.0f));
    camera_->set_aspect_ratio(SCREEN_WIDTH / SCREEN_HEIGHT);
    camera_->update_view();
    camera_->update_projection();
   
    controls_ = std::make_unique<scene::OrbitalCameraControls>(window_->get_glfw_ref(), camera_, glm::vec3(0.0f, 0.0f, 0.0f));
    camera_input_adapter_ = std::make_unique<scene::CameraInputAdapter>(controls_.get(), *camera_);
    window_->get_inputmanager()->add_listener(camera_input_adapter_.get());


    window_->set_resize_callback([this](int width, int height) {
            float aspect = static_cast<float>(width) / height;
            camera_->set_aspect_ratio(aspect);
            camera_->update_projection();
            });

    renderer_->init_grid(); 


    auto skybox_manager = std::make_shared<assets::SkyBoxManager> ();
    auto studio_skybox = init_hdri_skybox(skybox_manager, "studio", "textures/skybox/brown_photostudio.hdr");
    auto loft_skybox = init_hdri_skybox(skybox_manager, "loft", "textures/skybox/newport_loft.hdr");

    assets::ShaderLibrary::load("PBR", "shaders/pbr.vert", "shaders/pbr.frag");

    // shader has to be loaded before materials
    assets::MaterialLibrary::load_from_path("roofing", "assets/materials/roofing");
    assets::MaterialLibrary::load_from_path("bricks", "assets/materials/bricks");
    assets::MaterialLibrary::load_from_path("marble", "assets/materials/marble");
    assets::MaterialLibrary::load_from_path("gold", "assets/materials/gold");
    assets::MaterialLibrary::load_from_path("rock", "assets/materials/rock");

    // --- Set Scene, Init Render & UI ---
    scene_manager_->register_factory("main", [this, studio_skybox]() {
        auto scene = std::make_unique<MainScene>(renderer_.get(), camera_);
        scene->set_skybox(studio_skybox);
        return scene;
    });

    scene_manager_->register_factory("second", [this, loft_skybox]() {
        auto scene = std::make_unique<SecondScene>(renderer_.get(), camera_);
        scene->set_skybox(loft_skybox);
        return scene;
    });

    scene_manager_->set_scene("main");
    
    ui->initialize(window_->get_glfw_ref(), renderer_, scene_manager_, shared_from_this(), controls_, skybox_manager);

    return true;
}

void Engine::run() {
    window_->loop([this](float dt) {

        window_->get_inputmanager()->update(dt);

        renderer_->render_grid(*camera_, screen_width, screen_height);

        scene_manager_->update(dt);
        scene_manager_->render();

        window_->get_ui()->update();
        window_->get_ui()->render();
        scene_manager_->render_ui();

    });
}
}
