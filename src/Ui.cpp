#include "Ui.hpp"
#include "Renderer.hpp"
#include "SceneManager.hpp"
#include <GLFW/glfw3.h>
#include "Engine.hpp"

UI::UI() {}
UI::~UI() {}

void UI::initialize(GLFWwindow* window, const std::shared_ptr<Renderer> r, const std::shared_ptr<SceneManager> s, const std::shared_ptr<Engine> e, std::shared_ptr<Camera> c) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::SetNextWindowSize(ImVec2(300, 400));

    renderer = r;
    scene_manager = s;
    engine = e;
    camera = c;
}

void UI::render() {
}

void UI::update() {
    static bool prev_f1 = false;
    bool curr_f1 = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_F1) == GLFW_PRESS;

    if(curr_f1 && !prev_f1) {
        show_debug = !show_debug;
    }
    prev_f1 = curr_f1;
}



void UI::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

