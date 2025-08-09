#include "Engine.hpp"
#include <memory>
#include <glm/gtx/string_cast.hpp>
#include "SceneManager.hpp"
#include "MainScene.hpp"
#include "SecondScene.hpp"
#include "InputManager.hpp"
#include "Ui.hpp"
#include "Renderer.hpp"
#include <imgui_impl_opengl3.h>
#include "Window.hpp"
#include "Camera.hpp"
#include "SkyboxManager.hpp"
#include "SkyboxUtils.hpp"

#define SCREEN_WIDTH 1200.0f
#define SCREEN_HEIGHT 800.0f



Engine::Engine() {}
Engine::~Engine() {
    glfwDestroyWindow(window->get_glfw_ref());
    glfwTerminate();
}




bool Engine::init() {
    window = new Window(1200, 800, "Cosmos Engine", false, 4);
    window->set_clear_color(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    //window->enable_vsync();
    window->activate();
    window->init_ui();
    window->init_inputmanager();
    auto ui = window->get_ui();

    renderer = std::make_shared<Renderer>();
    scene_manager = std::make_shared<SceneManager>();
    renderer->set_ui(ui);


    camera_ = std::make_shared<Camera>();
    camera_->set_position(glm::vec3(0.0f, 0.0f, 10.0f));
    camera_->set_aspect_ratio(SCREEN_WIDTH / SCREEN_HEIGHT);
    camera_->update_view();
    camera_->update_projection();
   
    controls_ = std::make_unique<OrbitalCameraControls>(window->get_glfw_ref(), camera_, glm::vec3(0.0f, 0.0f, 0.0f));
    camera_input_adapter_ = std::make_unique<CameraInputAdapter>(controls_.get(), *camera_);
    window->get_inputmanager()->add_listener(camera_input_adapter_.get());


    window->set_resize_callback([this](int width, int height) {
            float aspect = static_cast<float>(width) / height;
            camera_->set_aspect_ratio(aspect);
            camera_->update_projection();
            });

    renderer->init_grid(); 

    auto skybox_manager = std::make_shared<SkyBoxManager> ();
    auto studio_skybox = init_hdri_skybox(skybox_manager, "studio", "textures/skybox/brown_photostudio.hdr");
    auto loft_skybox = init_hdri_skybox(skybox_manager, "loft", "textures/skybox/newport_loft.hdr");

    // --- Set Scene, Init Render & UI ---
    scene_manager->register_factory("main", [this, studio_skybox]() {
        auto scene = std::make_unique<MainScene>(renderer.get(), camera_);
        scene->set_skybox(studio_skybox);
        return scene;
    });
    scene_manager->register_factory("second", [this, loft_skybox]() {
        auto scene = std::make_unique<SecondScene>(renderer.get(), camera_);
        scene->set_skybox(loft_skybox);
        return scene;
    });
    scene_manager->set_scene("main");

    ui->initialize(window->get_glfw_ref(), renderer, scene_manager, shared_from_this(), controls_, skybox_manager);

    return true;
}

void Engine::run() {
    window->loop([this](float dt) {

        window->get_inputmanager()->update(dt);

        renderer->render_grid(*camera_, screen_width, screen_height);

        scene_manager->update(dt);
        scene_manager->render();

        window->get_ui()->update();
        window->get_ui()->render();
        scene_manager->render_ui();

    });
}
