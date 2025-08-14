#pragma once 
#include <memory>

namespace cosmos::core {
    class Engine; 
    class Window;
}
namespace cosmos::render{ class Renderer; }
namespace cosmos::scene { class SceneManager; class Camera; class CameraControls; class CameraInputAdapter; }
namespace cosmos::ui    { class UI; }
namespace cosmos::input { class InputManager; }
namespace cosmos::assets{ class SkyBoxManager; }


namespace cosmos::core {

// Visible to both Engine and App Bootstrap
struct EngineServices {
    core::Window& window;
    render::Renderer& renderer;
    scene::SceneManager& scene_manager;
    ui::UI& ui;
    input::InputManager& input_manager;
};

// Contstructed by AppBootStrap and given to scenes/UI
struct AppContext {
    std::shared_ptr<scene::Camera> camera;
    std::shared_ptr<scene::CameraControls> controls;     // TODO: make this union for controls typing (eg. Orbital, Fly, ...)
    std::shared_ptr<scene::CameraInputAdapter> camera_adapter;
    std::shared_ptr<assets::SkyBoxManager> skybox_manager;
};

struct IApp {
    virtual ~IApp() = default;
    virtual void bootstrap(cosmos::core::Engine& engine, const EngineServices& svc, AppContext& app ) = 0;
};

struct UiContext {
    core::Window& window;
    render::Renderer& renderer;
    scene::SceneManager& scene_manager;
    std::shared_ptr<scene::Camera> camera;
    std::shared_ptr<scene::CameraControls> controls;
    std::shared_ptr<scene::CameraInputAdapter> camera_adapter;
    std::shared_ptr<assets::SkyBoxManager> skybox_manager;
};


}
