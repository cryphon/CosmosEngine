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
std::shared_ptr<PerspectiveCamera> g_camera = nullptr; //forward declare

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

    engine->screen_width = width;
    engine->screen_height = height;

    if (g_camera) {
        g_camera->set_aspect_ratio(static_cast<float>(width) / height);
    }
}




Engine::Engine() {}
Engine::~Engine() {
    ui->shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Engine::init() {
    if (!glfwInit()) {
        LOG_ERROR("Failed to initialize GLFW");
        return false;
    }

    renderer = std::make_shared<Renderer>();
    scene_manager = std::make_shared<SceneManager>();
    ui = std::make_shared<UI>();
    renderer->set_ui(ui);


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D Space", nullptr, nullptr);
    if (!window) {
        LOG_ERROR("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);


   


    // --- Setup Camera ---
    camera = std::make_shared<PerspectiveCamera>(
    glm::vec3(0.0f, 1.0f, 3.0f),  // position
    glm::vec3(0.0f, 0.0f, 0.0f),  // target
    glm::vec3(0.0f, 1.0f, 0.0f)   // up
    );
    auto persp_camera = std::dynamic_pointer_cast<PerspectiveCamera>(camera);
    if (persp_camera) {
    persp_camera->update_view();
    persp_camera->set_aspect_ratio(SCREEN_WIDTH / SCREEN_HEIGHT);
    }
    g_camera = persp_camera;  // assign to global pointer for callback access


    
    // --- Setup Input Manager ---
    input = std::make_unique<InputManager>(window, persp_camera, scene_manager, renderer);

    // --- Load GLAD ---
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("Failed to initialize GLAD");
        return false;
    }
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    framebuffer_size_callback(window, w, h);

    glEnable(GL_DEPTH_TEST);
    LOG_DEBUG(std::string(format_mat4(camera->get_projection_matrix(), "Projection: ")));
    LOG_DEBUG("Starting skybox rendering...");
    // --- Set SkyBox
    std::vector<std::string> faces = {
    "textures/skybox/right.jpg",
    "textures/skybox/left.jpg",
    "textures/skybox/top.jpg",
    "textures/skybox/bottom.jpg",
    "textures/skybox/front.jpg",
    "textures/skybox/back.jpg"
    };

    auto capture_shader = std::make_shared<Shader>("shaders/hdr_to_cubemap.vert", "shaders/hdr_to_cubemap.frag");
    auto skybox_shader = std::make_shared<Shader>("shaders/skybox.vert", "shaders/skybox.frag");

    renderer->init_hdri_skybox("textures/skybox/brown_photostudio.hdr", capture_shader, skybox_shader);
    LOG_DEBUG("Skybox rendered");

    // --- required to reset the viewport after cubemap rendering for skybox ---
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);
    screen_width = w;
    screen_height = h;

    // --- Set Grid
    auto grid_shader = std::make_shared<Shader>("shaders/grid.vert", "shaders/grid.frag");
    renderer->init_grid(grid_shader);


    // --- Set Scene, Init Render & UI ---
    scene_manager->register_factory("main", [this]() {
        return std::make_unique<MainScene>(renderer.get(), camera);
    });
    scene_manager->register_factory("second", [this]() {
        return std::make_unique<SecondScene>(renderer.get(), camera);
    });
    scene_manager->set_scene("main");
    ui->initialize(window, renderer, scene_manager, shared_from_this(), camera);

    return true;
}

void Engine::run() {
    last_frame_time = std::chrono::high_resolution_clock::now();
    fps_timer = last_frame_time;

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
 
        renderer->render_skybox(*camera, screen_width, screen_height);
        renderer->render_grid(*camera, screen_width, screen_height);

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
