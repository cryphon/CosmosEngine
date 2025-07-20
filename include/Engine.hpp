#pragma once
#include <glad/glad.h>
#include <memory>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class UI;
class Renderer;
class SceneManager;
class InputManager;
class Camera;

class InputManager;

class Engine {
    public:
        Engine();
        ~Engine(); 
        int screen_width = 1000, screen_height = 1000;

        bool init();
        void run();
        InputManager* get_input();
    private:
        GLFWwindow* window;
        std::shared_ptr<Renderer> renderer;
        std::shared_ptr<SceneManager> scene_manager;
        std::shared_ptr<UI> ui;
        std::unique_ptr<InputManager> input;
        std::shared_ptr<Camera> camera;


        double prev_time = 0.0;

        void process_input();
        void update();
        void draw();
};


