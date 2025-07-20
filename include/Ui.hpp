#pragma once
#include <memory>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
class Renderer;
class Camera;

class UI {
public:
    UI();
    ~UI();

    void initialize(GLFWwindow* window, Renderer* r, std::shared_ptr<Camera> c);
    void render();
    void update();
    void shutdown();

private:
    Renderer* renderer = nullptr;
    std::shared_ptr<Camera> camera = nullptr;

    bool show_debug =false; 
};
