#include "Engine.hpp"
#include <iostream>
#include <chrono>
#include <memory>
#include <glm/gtx/string_cast.hpp>
#include "PerspectiveCamera.hpp"
#include "SceneManager.hpp"
#include "MainScene.hpp"
#include "SecondScene.hpp"
#include "InputManager.hpp"
#include "Ui.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Logger.hpp"

#define SCREEN_WIDTH 1200.0f
#define SCREEN_HEIGHT 800.0f


// TODO: move this to other file or smth
float last_x = 400.0f, last_y = 400.0f;
bool first_mouse = true;
bool rotate_drag = false;   // true while holding RMB
bool first_drag = true;     // reset delta to avoid jump

InputManager* Engine::get_input() {
    return input.get();
}

InputManager* get_input(GLFWwindow* window) {
    Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
    return engine ? engine->get_input() : nullptr;
}



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


static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    auto engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (!engine || height == 0) return;

    engine->on_resize(width, height);
}



bool Engine::init_glfw() {
    if (!glfwInit()) {
        LOG_ERROR("Failed to initialize GLFW");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    return true;
}

bool Engine::create_window() {
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D Space", nullptr, nullptr);
    if (!window) {
        LOG_ERROR("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return true;
}

bool Engine::init_glad() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("Failed to initialize GLAD");
        return false;
    }
    glEnable(GL_DEPTH_TEST);
    return true;
}

void Engine::init_renderer() {
    renderer = std::make_shared<Renderer>();
}

void Engine::init_input() {
    auto base_scene = scene_manager->get_current_scene_obj();
    auto renderable = dynamic_cast<RenderableScene*>(base_scene);
    std::shared_ptr<Camera> active_cam = renderable ? renderable->get_active_camera() : nullptr;    
    input = std::make_unique<InputManager>(window, active_cam, scene_manager, renderer);
}

void Engine::init_scene_factories() {
    scene_manager = std::make_shared<SceneManager>();
    scene_manager->register_factory("main", [this]() {
        return std::make_unique<MainScene>(renderer.get());
    });
    scene_manager->register_factory("second", [this]() {
        return std::make_unique<SecondScene>(renderer.get());
    });
    scene_manager->set_scene("main");
    resize_viewport();

}

void Engine::init_ui() {
    auto base_scene = scene_manager->get_current_scene_obj();
    auto renderable = dynamic_cast<RenderableScene*>(base_scene);
    std::shared_ptr<Camera> active_cam = renderable ? renderable->get_active_camera() : nullptr;
    ui = std::make_shared<UI>();
    renderer->set_ui(ui);
    ui->initialize(window, renderer, scene_manager, shared_from_this(), active_cam);
}

void Engine::init_skybox() {
    auto capture_shader = std::make_shared<Shader>("shaders/hdr_to_cubemap.vert", "shaders/hdr_to_cubemap.frag");
    auto skybox_shader = std::make_shared<Shader>("shaders/skybox.vert", "shaders/skybox.frag");

    renderer->init_hdri_skybox("textures/skybox/brown_photostudio.hdr", capture_shader, skybox_shader);
    LOG_DEBUG("Skybox rendered");
}

void Engine::init_grid() {
    auto grid_shader = std::make_shared<Shader>("shaders/grid.vert", "shaders/grid.frag");
    renderer->init_grid(grid_shader);
}

void Engine::resize_viewport() {
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    framebuffer_size_callback(window, w, h);
    glViewport(0, 0, w, h);
    screen_width = w;
    screen_height = h;
}

void Engine::on_resize(int width, int height) {
    screen_width = width;
    screen_height = height;
    auto base_scene = scene_manager->get_current_scene_obj();
    auto renderable = dynamic_cast<RenderableScene*>(base_scene);
    std::shared_ptr<Camera> active_cam = renderable ? renderable->get_active_camera() : nullptr;


    if (active_cam) {
        active_cam->set_aspect_ratio(static_cast<float>(width) / height);
    }
}




Engine::Engine() {}
Engine::~Engine() {
    ui->shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Engine::init() {
    if (!init_glfw()) return false;
    if (!create_window()) return false;
    if (!init_glad()) return false;

    init_renderer();
    init_scene_factories();   
    init_input();
    init_ui();
    init_skybox();
    init_grid();


    resize_viewport();
    LOG_DEBUG("Engine initialized successfully");
    return true;
}


void Engine::run() {
    last_frame_time = std::chrono::high_resolution_clock::now();
    fps_timer = last_frame_time;
    auto base_scene = scene_manager->get_current_scene_obj();
    auto renderable = dynamic_cast<RenderableScene*>(base_scene);
    std::shared_ptr<Camera> active_cam = renderable ? renderable->get_active_camera() : nullptr;


    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // --- Update last_frame and dt
        auto now = std::chrono::high_resolution_clock::now();
        delta_t = std::chrono::duration<float>(now - last_frame_time).count();
        last_frame_time = now;

        // --- ImGui Frame Start ---
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        input->update(delta_t);
 
        renderer->render_skybox(*active_cam, screen_width, screen_height);
        renderer->render_grid(*active_cam, screen_width, screen_height);

        scene_manager->update(delta_t);
        scene_manager->render();

        ui->update();
        ui->render();

        scene_manager->render_ui();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // --- Update FPS ---
        frame_count++;
        float elapsed = std::chrono::duration<float>(now - fps_timer).count();
        if (elapsed >= 1.0f) {
            fps = frame_count;
            frame_count = 0;
            fps_timer = now;
        }


        // --- ImGui Frame End ---
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
