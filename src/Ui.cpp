#include "Ui.hpp"
#include "Renderer.hpp"
#include "PerspectiveCamera.hpp"
#include "SceneManager.hpp"
#include "RenderableScene.hpp"
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
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Debug")) {
            ImGui::MenuItem("Show Debug", nullptr, &show_debug);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Scene Manager")) {
            static std::vector<std::string> scene_names = scene_manager->get_scene_names();
            std::string current_scene = scene_manager->get_current_scene_name();

            // Find index of current scene
            int current_index = 0;
            for (int i = 0; i < static_cast<int>(scene_names.size()); ++i) {
                if (scene_names[i] == current_scene) {
                    current_index = i;
                    break;
                }
            }

            if (ImGui::Combo("Select Scene", &current_index,
                        [](void* data, int idx, const char** out_text) {
                        const auto& names = *static_cast<std::vector<std::string>*>(data);
                        if (idx < 0 || idx >= static_cast<int>(names.size())) return false;
                        *out_text = names[idx].c_str();
                        return true;
                        }, static_cast<void*>(&scene_names), static_cast<int>(scene_names.size()))) {
                scene_manager->set_scene(scene_names[current_index]);
            }


            if (ImGui::Button("Reload Scene")) {
                scene_manager->reset_scene();
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImVec2 display_size = ImGui::GetIO().DisplaySize;
    float panel_width = 300.0f;
    static int selected_index = -1;

    // Set the position and size of the right panel
    ImGui::SetNextWindowPos(ImVec2(display_size.x - panel_width, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(panel_width, display_size.y), ImGuiCond_Always);

    // Optional flags: NoResize, NoMove, NoCollapse to make it truly fixed
    ImGuiWindowFlags panelFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Scene Objects", nullptr, panelFlags);

    try {
        RenderableScene* scene = static_cast<RenderableScene*>(scene_manager->get_current_scene_obj());
        auto objects = scene->get_objects();
        // List your scene objects here

        for (int i = 0; i < objects.size(); ++i) {
            bool is_selected = (i == selected_index);
            if (ImGui::Selectable(objects[i].name, is_selected)) {
                selected_index = i;
            }
        }
        ImGui::Separator();

        if (selected_index >= 0 && selected_index < objects.size()) {
            SceneObject obj = objects[selected_index];
            ImGui::Text("Editing: %s", obj.name);
        }

        ImGui::End();
    } catch(int err) { }

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
        ImGui::Text("FPS: %d", engine->get_fps());
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

