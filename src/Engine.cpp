#include "Engine.hpp"
#include <iostream>
#include "PerspectiveCamera.hpp"
#include "MainScene.hpp"


// TODO: move this to other file or smth
float last_x = 400.0f, last_y = 400.0f;
bool first_mouse = true;
float delta_time = 0.0f;
float last_frame = 0.0f;
bool rotate_drag = false;   // true while holding RMB
bool first_drag = true;     // reset delta to avoid jump


std::shared_ptr<PerspectiveCamera> g_camera = nullptr; //forward declare

Engine::Engine() {}
Engine::~Engine() {
    ui.shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Engine::init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 800, "3D Space", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int w, int h) {
        glViewport(0, 0, w, h);
    });

    // --- Setup Camera ---
    camera = std::make_shared<PerspectiveCamera>(
    glm::vec3(0.0f, 1.0f, 3.0f),  // position
    glm::vec3(0.0f, 0.0f, 0.0f),  // target
    glm::vec3(0.0f, 1.0f, 0.0f)   // up
    );
    auto persp_camera = std::dynamic_pointer_cast<PerspectiveCamera>(camera);
    if (persp_camera) persp_camera->update_direction();
    g_camera = persp_camera;  // assign to global pointer for callback access

    // --- Setup Input Manager ---
    input = std::make_unique<InputManager>(window, persp_camera);

    // --- Load GLAD ---
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glEnable(GL_DEPTH_TEST);


    // --- Set Scene, Init Render & UI ---
    scene_manager.set_scene(std::make_unique<MainScene>(&renderer, camera));

    // --- Set SkyBox
    std::vector<std::string> faces = {
    "textures/skybox/right.jpg",
    "textures/skybox/left.jpg",
    "textures/skybox/top.jpg",
    "textures/skybox/bottom.jpg",
    "textures/skybox/front.jpg",
    "textures/skybox/back.jpg"
    };
    auto skybox_shader = std::make_shared<Shader>("shaders/skybox.vert", "shaders/skybox.frag");
    renderer.init_skybox(faces, skybox_shader);



    ui.initialize(window);


    prev_time = glfwGetTime();
    return true;
}

void Engine::run() {
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // --- ImGui Frame Start ---
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        input->update(delta_time);


        renderer.render_skybox(*camera, 1000, 1000);

        scene_manager.update(delta_time);
        scene_manager.render();
        scene_manager.render_ui();

        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // --- ImGui Frame End ---
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
