#pragma once
#include <glad/glad.h>
#include <memory>
#include <chrono>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class UI;
class Renderer;
class SceneManager;
class InputManager;
class Camera;

class InputManager;

class Engine : public std::enable_shared_from_this<Engine>{
    public:
        Engine();
        ~Engine(); 
        int screen_width = 1000, screen_height = 1000;

        bool init();
        void run();
        InputManager* get_input();

        float get_delta_time() const { return delta_t; }
        int get_fps() const { return fps; }
        std::shared_ptr<Engine> get_ptr() { return shared_from_this(); }

    private:
        GLFWwindow* window;
        std::shared_ptr<Renderer> renderer;
        std::shared_ptr<SceneManager> scene_manager;
        std::shared_ptr<UI> ui;
        std::unique_ptr<InputManager> input;
        std::shared_ptr<Camera> camera;


        // --- Timing ---
        std::chrono::high_resolution_clock::time_point last_frame_time;
        float delta_t = 0.0f;



        // --- FPS tracking ---
        std::chrono::high_resolution_clock::time_point fps_timer;
        int frame_count;
        int fps;

        void process_input();
        void update();
        void draw();
};


