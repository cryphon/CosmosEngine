#pragma once
#include <memory>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"

class Renderer;
class CameraControls;
class SceneManager;
class Engine;
class Window;
class SceneObject;

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

    void initialize(GLFWwindow* window, const std::shared_ptr<Renderer> r, const std::shared_ptr<SceneManager> s, std::shared_ptr<Engine> e, std::shared_ptr<CameraControls> c);
    void render();
    void update();
    void shutdown();
    void set_camera_controls(std::shared_ptr<CameraControls> controls) { camera_controls = controls; }
    void set_renderer(std::shared_ptr<Renderer> renderer);
    void set_engine(std::shared_ptr<Engine> engine);
    void set_window(std::shared_ptr<Window> window);
    void show_shader_settings_popup(SceneObject& obj);

private:
    std::shared_ptr<Renderer> renderer = nullptr;
    std::shared_ptr<SceneManager> scene_manager = nullptr;
    std::shared_ptr<Engine> engine = nullptr;
    std::shared_ptr<CameraControls> camera_controls = nullptr;


    bool show_debug =false; 
};
