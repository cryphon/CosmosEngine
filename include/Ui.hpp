#pragma once
#include <memory>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
class Renderer;
class Camera;
class SceneManager;
class Engine;

class UI {
public:
    UI();
    ~UI();
    float reflectivity_slider = 0.5f;
    float alpha_slider = 0.5f;

    void initialize(GLFWwindow* window, const std::shared_ptr<Renderer> r, const std::shared_ptr<SceneManager> s, std::shared_ptr<Engine> e, std::shared_ptr<Camera> c);
    void render();
    void update();
    void shutdown();

private:
    std::shared_ptr<Renderer> renderer = nullptr;
    std::shared_ptr<SceneManager> scene_manager = nullptr;
    std::shared_ptr<Engine> engine = nullptr;
    std::shared_ptr<Camera> camera = nullptr;


    bool show_debug =false; 
};
