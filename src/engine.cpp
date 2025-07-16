#include "engine.hpp"
#include <iostream>

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


    std::cout << "Window: " << window << std::endl;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    std::cout << "GLAD loaded" << std::endl;

    glEnable(GL_DEPTH_TEST);

    renderer.initialize();
    ui.initialize(window);
    prev_time = glfwGetTime();
    return true;
}

void Engine::run() {
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ui.render(cam_x, cam_y, cam_z, scale, rotation_speed);

        double curr_time = glfwGetTime();
        if (curr_time - prev_time >= 1.0 / 60.0) {
            rotation += rotation_speed;
            prev_time = curr_time;
        }

        renderer.render(scale, rotation, glm::vec3(cam_x, cam_y, cam_z), 800, 800);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
