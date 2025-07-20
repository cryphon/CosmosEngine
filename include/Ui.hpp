#pragma once
#include <memory>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
class Renderer;
class Camera;
class SceneManager;

class UI {
public:
    UI();
    ~UI();

    void initialize(GLFWwindow* window, const std::shared_ptr<Renderer> r, const std::shared_ptr<SceneManager> s,  std::shared_ptr<Camera> c);
    void render();
    void update();
    void shutdown();

private:
    std::shared_ptr<Renderer> renderer = nullptr;
    std::shared_ptr<SceneManager> scene_manager = nullptr;
    std::shared_ptr<Camera> camera = nullptr;


    bool show_debug =false; 
};
