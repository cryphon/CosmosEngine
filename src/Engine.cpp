#include "Engine.hpp"
#include <iostream>
#include <chrono>
#include <memory>
#include <glm/gtx/string_cast.hpp>
#include "SceneManager.hpp"
#include "MainScene.hpp"
#include "SecondScene.hpp"
#include "Ui.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Logger.hpp"
#include "Window.hpp"
#include "Camera.hpp"
#include "CameraControls.hpp"

#define SCREEN_WIDTH 1200.0f
#define SCREEN_HEIGHT 800.0f


// TODO: move this to other file or smth
float last_x = 400.0f, last_y = 400.0f;
bool first_mouse = true;
bool rotate_drag = false;   // true while holding RMB
bool first_drag = true;     // reset delta to avoid jump


std::string format_mat4(const glm::mat4& mat, const std::string& label = "mat4x4") {
    std::ostringstream oss;
    oss << label << "(\n";
    for (int row = 0; row < 4; ++row) {
        oss << "  ";
        for (int col = 0; col < 4; ++col) {
            oss << std::setw(10) << std::fixed << std::setprecision(4) << mat[col][row];
            if (col < 3) oss << ", ";
        }
        oss << "\n";
    }
    oss << ")";
    return oss.str();
}


Engine::Engine() {}
Engine::~Engine() {
    ui->shutdown();
    glfwTerminate();
}

bool Engine::init() {
    // --- Create window and set OpenGL context
    window = std::make_shared<Window>(1200, 800, "Cosmos Engine", false, 4);
    window->set_clear_color(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    window->set_mousebutton_behaviour(MouseButtonBehaviour::NONE);
    window->activate();  // glfwMakeContextCurrent

    // --- Ensure correct viewport on startup
    auto size = window->get_size();
    glViewport(0, 0, size.width, size.height);

    // --- Setup camera and controls
    Transform cam_transform;
    cam_transform.position = glm::vec3(-7.0f, 3.0f, 7.0f);

    auto cam = std::make_shared<Camera>(cam_transform);
    cam->look_at(glm::vec3(0.0f, 0.0f, 0.0f));

    cameracontrols = std::make_shared<FlyCameraControls>();
    cameracontrols->set_speed(10.0f);
    camera = cam;  // assign to member before using it
    window->bind_camera(camera);
    window->bind_cameracontrols(cameracontrols);

    // --- Set correct aspect ratio once (even if resize callback hasn’t fired yet)
    camera->set_aspect_ratio(size);

    // --- Setup rendering + scene system
    renderer = std::make_shared<Renderer>();
    scene_manager = std::make_shared<SceneManager>();
    ui = std::make_shared<UI>();
    renderer->set_ui(ui);

    // --- Load shaders and skybox
    auto capture_shader = std::make_shared<Shader>(
        "shaders/hdr_to_cubemap.vert", "shaders/hdr_to_cubemap.frag");
    auto skybox_shader = std::make_shared<Shader>(
        "shaders/skybox.vert", "shaders/skybox.frag");

    renderer->init_hdri_skybox("textures/skybox/brown_photostudio.hdr", capture_shader, skybox_shader);
    LOG_DEBUG("Skybox initialized");

    // --- Init world grid
    auto grid_shader = std::make_shared<Shader>("shaders/grid.vert", "shaders/grid.frag");
    renderer->init_grid(grid_shader);

    // --- Set up scenes
    scene_manager->register_factory("second", [this]() {
        return std::make_unique<SecondScene>(renderer.get(), camera);
    });
    scene_manager->set_scene("second");

    // --- Init UI
    ui->initialize(window->get_glfw_ref(), renderer, scene_manager, shared_from_this(), camera);

    return true;
}


void Engine::run() {
    last_frame_time = std::chrono::high_resolution_clock::now();
    fps_timer = last_frame_time;
    frame_count = 0;

    window->loop([this](float delta_time) {
        delta_t = delta_time;

        // --- Start new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // --- Camera matrix updates
        if (renderer && camera) {
            camera->get_transform().update_matrices();

            // Optional: force aspect ratio update per frame (in case of resize)
            camera->set_aspect_ratio(window->get_size());

            renderer->render_skybox(*camera, window->get_size().width, window->get_size().height);
            renderer->render_grid(*camera, window->get_size().width, window->get_size().height);
        }


        // --- Scene logic
        if (scene_manager) {
            scene_manager->update(delta_time);
            scene_manager->render();
            scene_manager->render_ui();
        }

        // --- UI updates
        if (ui) {
            ui->update();
            ui->render();
        }

        // --- Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // --- FPS tracking
        frame_count++;
        auto now = std::chrono::high_resolution_clock::now();
        float elapsed = std::chrono::duration<float>(now - fps_timer).count();
        if (elapsed >= 1.0f) {
            fps = frame_count;
            frame_count = 0;
            fps_timer = now;
            LOG_INFO("FPS: " + std::to_string(fps));
        }
    });
}

