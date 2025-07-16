#pragma once
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

class UI {
public:
    UI();
    ~UI();

    void initialize(GLFWwindow* window);
    void render(float& x, float& y, float& z, float& scale);
    void shutdown();
};
