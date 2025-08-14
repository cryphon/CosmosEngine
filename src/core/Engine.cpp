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


bool Engine::init(const EngineConfig& config, IApp& app) {
    cfg_ = config;
    window_ = std::make_unique<Window>(cfg_.width, cfg_.height, cfg_.title, cfg_.fullscreen, cfg_.msaa);
    window_->set_clear_color(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    //window_->enable_vsync();
    window_->activate();
    window_->init_ui();
    window_->init_inputmanager();

    renderer_ = std::make_shared<render::Renderer>();
    scene_manager_ = std::make_shared<scene::SceneManager>();
    renderer_->set_ui(window_->get_ui());

    EngineServices svc{
        *window_,
        *renderer_,
        *scene_manager_,
        *window_->get_ui(),
        *window_->get_inputmanager()
    };

    app_ctx_ = std::make_shared<AppContext>();
    app.bootstrap(*this, svc, *app_ctx_);  

    return true;
}

void Engine::run() {
    window_->loop([this](float dt) {

        window_->get_inputmanager()->update(dt);


        scene_manager_->update(dt);
        scene_manager_->render();
        renderer_->render_grid(*app_ctx_->camera, cfg_.width, cfg_.height);

        window_->get_ui()->update();
        window_->get_ui()->render();
        scene_manager_->render_ui();

    });
}
}
