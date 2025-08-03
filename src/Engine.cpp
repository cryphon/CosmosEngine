#include "Engine.hpp"
#include <memory>
#include <glm/gtx/string_cast.hpp>
#include "PerspectiveCamera.hpp"
#include "SceneManager.hpp"
#include "MainScene.hpp"
#include "SecondScene.hpp"
#include "InputManager.hpp"
#include "Ui.hpp"
#include "Renderer.hpp"
#include <imgui_impl_opengl3.h>
#include "Window.hpp"

#define SCREEN_WIDTH 1200.0f
#define SCREEN_HEIGHT 800.0f


std::shared_ptr<PerspectiveCamera> g_camera = nullptr; //forward declare

Engine::Engine() {}
Engine::~Engine() {
    glfwDestroyWindow(window->get_glfw_ref());
    glfwTerminate();
}


void Engine::create_camera() {
    camera = std::make_shared<PerspectiveCamera>(
    glm::vec3(0.0f, 1.0f, 3.0f),  // position
    glm::vec3(0.0f, 0.0f, 0.0f),  // target
    glm::vec3(0.0f, 1.0f, 0.0f)   // up
    );
    auto persp_camera = std::dynamic_pointer_cast<PerspectiveCamera>(camera);
    if (persp_camera) {
    persp_camera->update_view();
    persp_camera->set_aspect_ratio(SCREEN_WIDTH / SCREEN_HEIGHT);
    }
    g_camera = persp_camera;  // assign to global pointer for callback acces TODO: NOT THIS, REVISE CAMERA IN GENERAL
}


bool Engine::init() {
    window = new Window(1200, 800, "Cosmos Engine", false, 4);
    window->set_clear_color(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    window->enable_vsync();
    window->activate();
    window->init_ui();
    window->init_inputmanager();
    auto ui = window->get_ui();

    renderer = std::make_shared<Renderer>();
    scene_manager = std::make_shared<SceneManager>();
    renderer->set_ui(ui);

    create_camera();
                      
    window->get_inputmanager()->set_camera(g_camera);
    window->get_inputmanager()->set_renderer(renderer);
    window->get_inputmanager()->set_scenemanager(scene_manager);

    window->set_resize_callback([this](int width, int height) {
            float aspect = static_cast<float>(width) / height;
            camera->set_aspect_ratio(aspect);
            camera->update_projection();
            });

    renderer->init_hdri_skybox("textures/skybox/brown_photostudio.hdr");
    renderer->init_grid();


    // --- Set Scene, Init Render & UI ---
    scene_manager->register_factory("main", [this]() {
        return std::make_unique<MainScene>(renderer.get(), camera);
    });
    scene_manager->register_factory("second", [this]() {
        return std::make_unique<SecondScene>(renderer.get(), camera);
    });
    scene_manager->set_scene("main");
    ui->initialize(window->get_glfw_ref(), renderer, scene_manager, shared_from_this(), camera);

    return true;
}

void Engine::run() {
    window->loop([this](float dt) {
            delta_t = dt;

            window->get_inputmanager()->update(dt);

            renderer->render_skybox(*camera, screen_width, screen_height);
            renderer->render_grid(*camera, screen_width, screen_height);

            scene_manager->update(dt);
            scene_manager->render();

            window->get_ui()->update();
            window->get_ui()->render();
            scene_manager->render_ui();

    });
}
