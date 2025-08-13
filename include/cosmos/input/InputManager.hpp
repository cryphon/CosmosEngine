#pragma once
// ==
// Standard Library
// ==
#include <memory>
#include <vector>
// ==
// Third Party
// ==
#include <GLFW/glfw3.h>

// ==
// Cosmos
// ==

// ==
// Forward Declare
// ==
namespace cosmos::render { class Renderer; }
namespace cosmos::scene { class Camera; class SceneManager; }
namespace cosmos::input { class InputListener; }


namespace cosmos::input {

class InputManager {
public:
    InputManager(GLFWwindow* window);

    void update(float dt);
    void set_input_cbs();

    void add_listener(InputListener* listener);

    void set_camera(std::shared_ptr<scene::Camera> cam);
    void set_renderer(std::shared_ptr<render::Renderer> ren);
    void set_scenemanager(std::shared_ptr<scene::SceneManager> manager);

    int get_selected_object_id();

private:
    GLFWwindow* window_;
    std::shared_ptr<scene::Camera> camera_;
    std::shared_ptr<scene::SceneManager> scene_manager_;
    std::shared_ptr<render::Renderer> renderer_;

    std::vector<InputListener*> listeners_;

    // Callback dispatchers
    static void glfw_cursor_cb(GLFWwindow* win, double xpos, double ypos);
    static void glfw_mouse_button_cb(GLFWwindow* win, int button, int action, int mods);
    static void glfw_key_cb(GLFWwindow* win, int key, int scancode, int action, int mods);
    static void glfw_scroll_cb(GLFWwindow* win, double xoffset, double yoffset);

    // Actual dispatcher methods
    void dispatch_mouse_move(double xpos, double ypos);
    void dispatch_mouse_button(int button, int action, int mods);
    void dispatch_key(int key, int action, int mods);
    void dispatch_scroll(double xoffset, double yoffset);
};
}

