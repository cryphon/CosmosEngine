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
#include <imgui_impl_opengl3.h>
#include "Window.hpp"
#include "Camera.hpp"

#define SCREEN_WIDTH 1200.0f
#define SCREEN_HEIGHT 800.0f


std::shared_ptr<PerspectiveCamera> g_camera = nullptr; //forward declare


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
    glfwDestroyWindow(window->get_glfw_ref());
    glfwTerminate();
}


void Engine::create_camera() {
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
    g_camera = persp_camera;  // assign to global pointer for callback acces TODO: NOT THIS, REVISE CAMERA IN GENERAL
}


bool Engine::init() {
    window = new Window(1200, 800, "Cosmos Engine", false, 4);
    window->set_clear_color(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    window->enable_vsync();
    window->activate();
    window->init_ui();
    window->init_inputmanager();
    auto ui = window->get_ui();

    renderer = std::make_shared<Renderer>();
    scene_manager = std::make_shared<SceneManager>();
    renderer->set_ui(ui);

    create_camera();
                      
    window->get_inputmanager()->set_camera(g_camera);
    window->get_inputmanager()->set_renderer(renderer);
    window->get_inputmanager()->set_scenemanager(scene_manager);

    window->set_resize_callback([this](int width, int height) {
            float aspect = static_cast<float>(width) / height;
            camera->set_aspect_ratio(aspect);
            camera->update_projection();
            });


    LOG_DEBUG(std::string(format_mat4(camera->get_projection_matrix(), "Projection: ")));
    LOG_DEBUG("Starting skybox rendering...");

    auto capture_shader = std::make_shared<Shader>("shaders/hdr_to_cubemap.vert", "shaders/hdr_to_cubemap.frag");
    auto skybox_shader = std::make_shared<Shader>("shaders/skybox.vert", "shaders/skybox.frag");

    renderer->init_hdri_skybox("textures/skybox/brown_photostudio.hdr", capture_shader, skybox_shader);
    LOG_DEBUG("Skybox rendered");
 
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
    ui->initialize(window->get_glfw_ref(), renderer, scene_manager, shared_from_this(), camera);

    return true;
}

void Engine::run() {
    last_frame_time = std::chrono::high_resolution_clock::now();
    fps_timer = last_frame_time;

    while (!glfwWindowShouldClose(window->get_glfw_ref())) {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);


        // Clear the appropriate buffers.
        auto clearBits = GL_COLOR_BUFFER_BIT;
        if (window->depthtest_enabled) {
            clearBits |= GL_DEPTH_BUFFER_BIT;
        }
        if (window->stenciltest_enabled) {
            clearBits |= GL_STENCIL_BUFFER_BIT;
        }
        glClear(clearBits);


        // --- Update last_frame and dt
        auto now = std::chrono::high_resolution_clock::now();
        delta_t = std::chrono::duration<float>(now - last_frame_time).count();
        last_frame_time = now;

        // --- ImGui Frame Start ---
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        window->get_inputmanager()->update(delta_t);
 
        renderer->render_skybox(*camera, screen_width, screen_height);
        renderer->render_grid(*camera, screen_width, screen_height);

        scene_manager->update(delta_t);
        scene_manager->render();

        window->get_ui()->update();
        window->get_ui()->render();

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
        glfwSwapBuffers(window->get_glfw_ref());
        glfwPollEvents();
    }
}
