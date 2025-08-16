#pragma once
// ==
// Standard Library
// ==
#include <memory>

// ==
// Third Party
// ==
#include <glad/glad.h>

// ==
// Cosmos
// ==
#include <cosmos/core/AppAPI.hpp>
#include <cosmos/core/Window.hpp>

// ==
// Forward Declare
// ==
namespace cosmos::ui { class UI; }
namespace cosmos::render { class Renderer; }
namespace cosmos::scene { class SceneManager; class Camera; class CameraControls; class CameraInputAdapter; }
namespace cosmos::input { class InputManager; }
namespace cosmos::assets { class SkyBoxManager; }

namespace cosmos::core {

struct EngineConfig {
    int width = 1000;
    int height = 1000;
    bool fullscreen = false;
    int msaa = 4;
    const char* title = "cosmos";
};


class Engine : public std::enable_shared_from_this<Engine>{
public:
    Engine();
    ~Engine(); 

    bool init(const EngineConfig& cfg, IApp& app);
    void run();


    Window& get_window() { return *window_; }
    render::Renderer& get_renderer() { return *renderer_; }
    scene::SceneManager& get_scene_manager() { return *scene_manager_; }
    ui::UI& get_ui() { return *window_->get_ui(); }
    input::InputManager& get_inputmanager() { return *window_->get_inputmanager(); }
    scene::Camera& get_camera() { return *camera_; }
    scene::CameraControls& get_camera_controls() { return *controls_; }
    scene::CameraInputAdapter& get_camera_adapter() { return *camera_adapter_; }


    std::shared_ptr<Engine> get_ptr() { return shared_from_this(); }

private:
    std::unique_ptr<Window> window_;
    std::shared_ptr<render::Renderer> renderer_;
    std::shared_ptr<scene::SceneManager> scene_manager_;
    std::shared_ptr<scene::Camera> camera_;
    std::shared_ptr<scene::CameraControls> controls_;
    std::shared_ptr<scene::CameraInputAdapter> camera_adapter_;
    std::shared_ptr<core::AppContext> app_ctx_;
    EngineConfig cfg_;




    void process_input();
    void update();
    void draw();
};

}


