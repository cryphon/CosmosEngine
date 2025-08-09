#include "Ui.hpp"
#include "Renderer.hpp"
#include "SceneManager.hpp"
#include "RenderableScene.hpp"
#include "ShaderLibrary.hpp"
#include <GLFW/glfw3.h>
#include "Engine.hpp"
#include <imgui_impl_opengl3.h>
#include "Logger.hpp"
#include "Camera.hpp"
#include "PBRMaterial.hpp"
#include "SkyboxManager.hpp"
#include <memory>
#include "MaterialLibrary.hpp"
#include "Texture.hpp"

UI::UI() {}
UI::~UI() {
    shutdown();
}

void UI::initialize(GLFWwindow* window, const std::shared_ptr<Renderer> r, const std::shared_ptr<SceneManager> s, const std::shared_ptr<Engine> e, std::shared_ptr<CameraControls> c, const std::shared_ptr<SkyBoxManager> sbm) {
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
    camera_controls = c;
    skybox_manager = sbm;
}

void UI::render() {
    if(camera_controls == nullptr) {
        LOG_ERROR("Initialize and add camera controls to UI before calling UI::render");
        exit(-1);
    }
    if(renderer == nullptr) {
        LOG_ERROR("Initialize and add renderer to UI before calling UI::render");
        exit(-1);
    }

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
        if (ImGui::BeginMenu("Skybox Manager")) {
            auto skybox_names = skybox_manager->get_skybox_names();
            std::string current_skybox = skybox_manager->get_current_skybox_name();

            // Find current skybox index
            int current_index = 0;
            for (int i = 0; i < static_cast<int>(skybox_names.size()); ++i) {
                if (skybox_names[i] == current_skybox) {
                    current_index = i;
                    break;
                }
            }

            if (ImGui::Combo("Select Skybox", &current_index,
                [](void* data, int idx, const char** out_text) {
                    const auto& names = *static_cast<std::vector<std::string>*>(data);
                    if (idx < 0 || idx >= static_cast<int>(names.size())) return false;
                    *out_text = names[idx].c_str();
                    return true;
                }, static_cast<void*>(&skybox_names), static_cast<int>(skybox_names.size()))) {

                RenderableScene* scene = dynamic_cast<RenderableScene*>(scene_manager->get_current_scene_obj());
                if (scene) {
                    skybox_manager->set_skybox(skybox_names[current_index]);

                    std::shared_ptr<SkyBox> skybox = std::shared_ptr<SkyBox>(skybox_manager->get_current_skybox());
                    scene->set_skybox(skybox);
                }
            }

            if (ImGui::Button("Reload Skybox")) {
                skybox_manager->set_skybox(current_skybox);
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
    ImGui::SetNextWindowSize(ImVec2(panel_width, display_size.y / 2), ImGuiCond_Always);

    // Optional flags: NoResize, NoMove, NoCollapse to make it truly fixed
    ImGuiWindowFlags panelFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Scene Objects", nullptr, panelFlags);
    RenderableScene* scene = static_cast<RenderableScene*>(scene_manager->get_current_scene_obj());


    try {
        auto& objects = scene->get_objects();
        // List your scene objects here

        for (int i = 0; i < objects.size(); ++i) {
            bool is_selected = (i == selected_index);
            if (ImGui::Selectable(objects[i].name.c_str(), is_selected)) {
                selected_index = i;
            }
        }
        ImGui::Separator();

        if (selected_index >= 0 && selected_index < objects.size()) {
            SceneObject& obj = objects[selected_index];
            ImGui::Text("Editing: %s", obj.name.c_str());


            bool changed = false;
            changed |= ImGui::DragFloat3("Position", glm::value_ptr(obj.transform.position), 0.1f);
            changed |= ImGui::DragFloat3("Rotation", glm::value_ptr(obj.transform.rotation), 0.5f);
            changed |= ImGui::DragFloat3("Scale", glm::value_ptr(obj.transform.scale), 0.05f);

            // Shader selection
            std::vector<std::string> shader_keys = ShaderLibrary::get_keys();
            std::string current_shader_name = ShaderLibrary::get_name(obj.material->shader);

            if (ImGui::BeginCombo("Shader", current_shader_name.c_str())) {
                for (const auto& name : shader_keys) {
                    bool selected = (current_shader_name == name);
                    if (ImGui::Selectable(name.c_str(), selected)) {
                        current_shader_name = name;
                        obj.material->shader = ShaderLibrary::get(name);
                    }
                    if (selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            // Open shader config window on button click
            if (ImGui::Button("Open Shader Settings")) {
                ImGui::OpenPopup("ShaderSettingsPopup");
            }

            // Show the popup or window
            show_shader_settings_popup(obj);

            if (changed) {
                obj.transform.cache_trigger = true;
                obj.transform.update_matrices();
            }
        }
        ImGui::End();
    } catch(int err) { }

    if (show_debug && camera_controls) {

        std::shared_ptr<CameraControls> base_ptr = camera_controls;
        std::shared_ptr<OrbitalCameraControls> orbital_ptr = std::dynamic_pointer_cast<OrbitalCameraControls>(base_ptr);

        if (orbital_ptr && passive_rotation == false) {
            yaw_slider = orbital_ptr->get_yaw();
            pitch_slider = orbital_ptr->get_pitch();
        }


        ImGui::Begin("Camera/Debug Options");
        bool changed = false;
        changed |= ImGui::Checkbox("Show Grid", &renderer->grid_enabled);
        changed |= ImGui::Checkbox("Show Skybox", &renderer->skybox_enabled);
        changed |= ImGui::SliderFloat("Yaw", &yaw_slider, -180.0f, 180.0f);
        changed |= ImGui::SliderFloat("Pitch", &pitch_slider, -89.0f, 89.0f);
        changed |= ImGui::Checkbox("Enable Passive Rotation", &passive_rotation);
        changed |= ImGui::SliderFloat("Passive Yaw Speed", &rotation_speed, -100.0f, 100.0f);
        ImGui::End();

        if (orbital_ptr && changed) {
            orbital_ptr->set_angles(yaw_slider, pitch_slider);
            orbital_ptr->apply_man_update();
            orbital_ptr->set_passive_rotation(passive_rotation);
            orbital_ptr->set_rotation_speed(rotation_speed);
        }    
    }

    ImGui::SetNextWindowPos(ImVec2(display_size.x - panel_width, display_size.y / 2), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(panel_width, display_size.y / 2), ImGuiCond_Always);


    if (ImGui::Begin("Material Editor")) {
        if (selected_index >= 0) {
            SceneObject& obj = scene->get_objects()[selected_index];
            auto mat = std::dynamic_pointer_cast<PBRMaterial>(obj.material);
            if (mat) {

                EditTextureSlot("Albedo Map", mat->useAlbedoMap, mat->albedoMap);
                if (!mat->albedoMap)
                    ImGui::ColorEdit3("Albedo Color", glm::value_ptr(mat->albedo));

                EditTextureSlot("Normal Map", mat->useNormalMap, mat->normalMap);
                EditTextureSlot("Roughness Map", mat->useRoughnessMap, mat->roughnessMap);
                if (!mat->roughnessMap)
                    ImGui::SliderFloat("Roughness", &mat->roughness, 0.0f, 1.0f);

                EditTextureSlot("Metallic Map", mat->useMetallicMap, mat->metallicMap);
                if (!mat->metallicMap)
                    ImGui::SliderFloat("Metallic", &mat->metallic, 0.0f, 1.0f);

                EditTextureSlot("AO Map", mat->useAoMap, mat->aoMap);
                EditTextureSlot("Displacement Map", mat->useDisplacementMap, mat->displacementMap);
                if (!mat->displacementMap)
                    ImGui::SliderFloat("Displacement", &mat->displacement, 0.0f, 1.0f);

                ImGui::SliderFloat("Tiling", &mat->tiling, 0.1f, 10.0f);
            }
        }
    }
    ImGui::End();
}


void UI::show_shader_settings_popup(SceneObject& obj) {
    if (ImGui::BeginPopup("ShaderSettingsPopup")) {
        std::string shader_name = ShaderLibrary::get_name(obj.material->shader);

        ImGui::Text("Shader: %s", shader_name.c_str());
        ImGui::Separator();

        // Example: Check for known uniforms based on shader name
        if (shader_name == "Reflective" || shader_name == "PBR") {
            ImGui::SliderFloat("Reflectivity", &reflectivity_slider , 0.0f, 1.0f);
            obj.material->shader->set_float("uReflectivity", reflectivity_slider);

            ImGui::SliderFloat("Alpha", &alpha_slider, 0.0f, 1.0f);
            obj.material->shader->set_float("uAlpha", alpha_slider);
        }

        if (shader_name == "PBR") {
            auto mat = std::dynamic_pointer_cast<PBRMaterial>(obj.material);
            ImGui::SliderFloat("Roughness", &roughness_slider, 0.0f, 1.0f);
            mat->roughness = roughness_slider;
            ImGui::SliderFloat("Metallic", &metallic_slider, 0.0f, 1.0f);
            mat->metallic = metallic_slider;
            ImGui::SliderFloat("Tiling", &tiling_slider, 0.1f, 10.0f);
            mat->tiling = tiling_slider;
            ImGui::SliderFloat("Displacement", &displacement_slider, 0.0f, 1.0f);
            mat->displacement = displacement_slider;
        }

        ImGui::EndPopup();
    }
}

bool UI::EditTextureSlot(const char* label, bool& use_map, std::shared_ptr<Texture>& texture, GLenum texture_target) {
    bool changed = false;

    if (ImGui::Checkbox(label, &use_map)) {
        if (!use_map) {
            changed = true;
        } else {
            // Set enabled, but don't load until user clicks button
        }
    }

    if (use_map) {
        if (texture) {
            ImGui::SameLine();
            ImGui::Image((ImTextureID)texture->ID, ImVec2(64, 64));
        }
    }

    return changed;
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

