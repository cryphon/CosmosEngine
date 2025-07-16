#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "renderer.hpp"
#include "ui.hpp"

class Engine {
    public:
        Engine();
        ~Engine();

        bool init();
        void run();

    private:
        GLFWwindow* window;
        Renderer renderer;
        UI ui;


        std::unique_ptr<Camera> camera;
        float scale = 1.0f;
        float rotation = 0.0f;
        float rotation_speed = 0.05f;
        double prev_time = 0.0;

        void process_input();
        void update();
        void draw();
};


