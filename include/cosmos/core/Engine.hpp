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

// ==
// Forward Declare
// ==
namespace cosmos::core { class Window; }
namespace cosmos::ui { class UI; }
namespace cosmos::render { class Renderer; }
namespace cosmos::scene { class SceneManager; class Camera; class CameraControls; class CameraInputAdapter; }
namespace cosmos::input { class InputManager; }

namespace cosmos::core {

class Engine : public std::enable_shared_from_this<Engine>{
public:
    Engine();
    ~Engine(); 
    int screen_width = 1000, screen_height = 1000;

    bool init();
    void run();
    void create_camera();
    input::InputManager* get_input();
    cosmos::core::Window* get_window() { return window_; }

    std::shared_ptr<Engine> get_ptr() { return shared_from_this(); }

private:
    cosmos::core::Window* window_;
    std::shared_ptr<render::Renderer> renderer_;
    std::shared_ptr<scene::SceneManager> scene_manager_;
    std::shared_ptr<scene::Camera> camera_;
    std::shared_ptr<scene::CameraControls> controls_;
    std::shared_ptr<scene::CameraInputAdapter> camera_input_adapter_;




    void process_input();
    void update();
    void draw();
};

}


