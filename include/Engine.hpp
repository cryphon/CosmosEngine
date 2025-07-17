#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Renderer.hpp"
#include "SceneManager.hpp"
#include "InputManager.hpp"
#include "Ui.hpp"

class Engine {
    public:
        Engine();
        ~Engine();

        bool init();
        void run();

    private:
        GLFWwindow* window;
        Renderer renderer;
        SceneManager scene_manager;
        std::unique_ptr<InputManager> input;
        std::shared_ptr<Camera> camera;
        UI ui;


        double prev_time = 0.0;

        void process_input();
        void update();
        void draw();
};


