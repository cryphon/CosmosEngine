#include "ui.hpp"

UI::UI() {}
UI::~UI() {}

void UI::initialize(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void UI::render(float& camX, float& camY, float& camZ, float& scale) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Settings");
    ImGui::SliderFloat("Camera X", &camX, -10.0f, 10.0f);
    ImGui::SliderFloat("Camera Y", &camY, -10.0f, 10.0f);
    ImGui::SliderFloat("Camera Z", &camZ, -10.0f, 10.0f);
    ImGui::SliderFloat("Pyramid Scale", &scale, 0.1f, 5.0f);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

