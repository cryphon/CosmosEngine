#include "Ui.hpp"
#include "Renderer.hpp"
#include "PerspectiveCamera.hpp"
#include <GLFW/glfw3.h>

UI::UI() {}
UI::~UI() {}

void UI::initialize(GLFWwindow* window, Renderer* r, std::shared_ptr<Camera> c) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::SetNextWindowSize(ImVec2(300, 400));

    renderer = r;
    camera = c;
}

void UI::render() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Debug")) {
            ImGui::MenuItem("Show Debug", nullptr, &show_debug);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (show_debug && camera) {
        ImGui::Begin("Camera Position");
        if (auto persp = dynamic_cast<PerspectiveCamera*>(camera.get())) {
            glm::vec3 pos = persp->get_position();
            if (ImGui::DragFloat3("Camera Pos", &pos.x, 0.1f)) {
                persp->set_position(pos);
                persp->update_view();
            }
        }
        ImGui::Checkbox("Show Grid", &renderer->grid_enabled);
        ImGui::Checkbox("Show Skybox", &renderer->skybox_enabled);
        ImGui::End();
    }
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

