#pragma once

#include <memory>
#include <vector>

class GLFWwindow;
class Renderer;
class Camera;
class InputListener;
class SceneManager;

class InputManager {
public:
    InputManager(GLFWwindow* window);

    void update(float dt);
    void set_input_cbs();

    void add_listener(InputListener* listener);

    void set_camera(std::shared_ptr<Camera> cam);
    void set_renderer(std::shared_ptr<Renderer> ren);
    void set_scenemanager(std::shared_ptr<SceneManager> manager);

    int get_selected_object_id();

private:
    GLFWwindow* window;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<SceneManager> scene_manager;
    std::shared_ptr<Renderer> renderer;

    std::vector<InputListener*> listeners;

    // Callback dispatchers
    static void glfw_cursor_cb(GLFWwindow* win, double xpos, double ypos);
    static void glfw_mouse_button_cb(GLFWwindow* win, int button, int action, int mods);
    static void glfw_key_cb(GLFWwindow* win, int key, int scancode, int action, int mods);

    // Actual dispatcher methods
    void dispatch_mouse_move(double xpos, double ypos);
    void dispatch_mouse_button(int button, int action, int mods);
    void dispatch_key(int key, int action, int mods);
};

