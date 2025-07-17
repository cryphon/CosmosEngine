#include "Engine.hpp"
#include <iostream>
#include "PerspectiveCamera.hpp"
#include "SceneManager.hpp"
#include "MainScene.hpp"

// TODO: move this to other file or smth
float last_x = 400.0f, last_y = 400.0f;
bool first_mouse = true;
float delta_time = 0.0f;
float last_frame = 0.0f;
bool rotate_drag = false;   // true while holding RMB
bool first_drag = true;     // reset delta to avoid jump

SceneManager scene_manager;

std::shared_ptr<PerspectiveCamera> g_camera = nullptr; //forward declare

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static float sensitivity = 0.003f; // slower look

    if (!rotate_drag) {
        // Not dragging: keep last positions up to date so we don't get a jump on next drag
        last_x = xpos;
        last_y = ypos;
        return;
    }

    if (first_drag) {
        last_x = xpos;
        last_y = ypos;
        first_drag = false;
    }

    float xoffset = float(xpos - last_x);
    float yoffset = float(last_y - ypos); // inverted y

    last_x = xpos;
    last_y = ypos;

    g_camera->yaw   += xoffset * sensitivity;
    g_camera->pitch += yoffset * sensitivity;

    if (g_camera->pitch > 89.0f)  g_camera->pitch = 89.0f;
    if (g_camera->pitch < -89.0f) g_camera->pitch = -89.0f;

    g_camera->update_direction();
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
   // using ImGui, ignore when it wants the mouse:
    if (ImGui::GetIO().WantCaptureMouse) return;

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            rotate_drag = true;
            first_drag = true;                // reset ref point
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else if (action == GLFW_RELEASE) {
            rotate_drag = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void process_input(GLFWwindow* window, PerspectiveCamera& cam) {
    float speed = 5.0f * delta_time;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.position += cam.front * speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.position -= cam.front * speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.position -= cam.right * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.position += cam.right * speed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cam.position -= cam.up * speed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cam.position += cam.up * speed;
}



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

    // register input and mouse callback
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


    // setup camera
    camera = std::make_shared<PerspectiveCamera>(
    glm::vec3(0.0f, 1.0f, 3.0f),  // position
    glm::vec3(0.0f, 0.0f, 0.0f),  // target
    glm::vec3(0.0f, 1.0f, 0.0f)   // up
    );
    auto persp_camera = std::dynamic_pointer_cast<PerspectiveCamera>(camera);
    if (persp_camera) persp_camera->update_direction();


    g_camera = persp_camera;  // assign to global pointer for callback access


    std::cout << "Window: " << window << std::endl;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    std::cout << "GLAD loaded" << std::endl;

    glEnable(GL_DEPTH_TEST);

    scene_manager.set_scene(std::make_unique<MainScene>(&renderer, camera));

    renderer.initialize();
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



        double curr_time = glfwGetTime();
        if (curr_time - prev_time >= 1.0 / 60.0) {
            rotation += rotation_speed;
            prev_time = curr_time;
        }

        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        if(g_camera) {
        ::process_input(window, *g_camera);
        }

        scene_manager.update(delta_time);
        scene_manager.render();
        scene_manager.render_ui();

        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
