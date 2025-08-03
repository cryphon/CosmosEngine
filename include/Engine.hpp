#pragma once
#include <glad/glad.h>
#include <memory>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Window.hpp"

class UI;
class Renderer;
class SceneManager;
class InputManager;
class Camera;
class CameraControls;
class CameraInputAdapter;

class InputManager;

class Engine : public std::enable_shared_from_this<Engine>{
    public:
        Engine();
        ~Engine(); 
        int screen_width = 1000, screen_height = 1000;

        bool init();
        void run();
        void create_camera();
        InputManager* get_input();
        Window* get_window() { return window; }

        std::shared_ptr<Engine> get_ptr() { return shared_from_this(); }

    private:
        Window* window;
        std::shared_ptr<Renderer> renderer;
        std::shared_ptr<SceneManager> scene_manager;
        std::shared_ptr<Camera> camera_;
        std::shared_ptr<CameraControls> controls_;
        std::shared_ptr<CameraInputAdapter> camera_input_adapter_;




        void process_input();
        void update();
        void draw();
};


