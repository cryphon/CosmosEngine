#pragma once
// ==
// Standard Library
// ==
#include <memory>

// ==
// Third Party
// ==
#include <glad/glad.h>
#include <imgui.h>
#include "backends/imgui_impl_glfw.h"

// ==
// Cosmos
// ==
#include <cosmos/core/AppAPI.hpp>
#include <cosmos/core/Profiler.hpp>
#include <cosmos/ui/PerfOverlay.hpp>

// ==
// Forward Declare
// == 
namespace cosmos::render { class Renderer; class Texture; }
namespace cosmos::core { class Engine; class Window; }
namespace cosmos::scene { class SceneObject; class CameraControls; class SceneManager; }
namespace cosmos::assets { class SkyBoxManager; }

namespace cosmos::ui {


class UI {
public:
    UI();
    ~UI();
    float reflectivity_slider = 0.5f;
    float alpha_slider = 0.5f;
    float roughness_slider = 0.0f;
    float metallic_slider = 0.0f;
    float yaw_slider = -90.0f;
    float pitch_slider = 0.0f;
    bool passive_rotation = false;
    float rotation_speed = 10.0f;
    float tiling_slider = 1.0f;
    float displacement_slider = 0.05f;

    void initialize(const core::UiContext& ctx);    
    void render();
    void update();
    void shutdown();
    void set_camera_controls(std::shared_ptr<scene::CameraControls> controls) { camera_controls_ = controls; }
    void set_renderer(std::shared_ptr<render::Renderer> renderer);
    void set_engine(std::shared_ptr<core::Engine> engine);
    void set_window(std::shared_ptr<core::Window> window);
    void show_shader_settings_popup(scene::SceneObject& obj);
    bool EditTextureSlot(const char* label, bool& use_map, std::shared_ptr<render::Texture>& texture, GLenum texture_target = GL_TEXTURE_2D);

private:
    core::Window* window_ = nullptr;  // non-owning pointer (comes from EngineServices)
    render::Renderer* renderer_ = nullptr;  // non-owning pointer
    scene::SceneManager* scene_manager_ = nullptr; // non-owning pointer
    core::Profiler* profiler_ = nullptr;
    PerfOverlayConfig perf_cfg_;

    std::shared_ptr<scene::Camera> camera_; // shared ownership with AppContext
    std::shared_ptr<scene::CameraControls> camera_controls_; // shared ownership
    std::shared_ptr<assets::SkyBoxManager> skybox_manager_; // shared ownership

    bool show_debug =false; 
};
}
