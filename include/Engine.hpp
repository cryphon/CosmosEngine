#pragma once
#include <glad/glad.h>
#include <memory>
#include <chrono>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "CameraControls.hpp"

class UI;
class Renderer;
class SceneManager;
class Camera;
class Window;


class Engine : public std::enable_shared_from_this<Engine>{
    public:
        Engine();
        ~Engine(); 

        bool init();
        void run();

        std::shared_ptr<Engine> get_ptr() { return shared_from_this(); }

    private:
        std::shared_ptr<Window> window;
        std::shared_ptr<Camera> camera;
        std::shared_ptr<Renderer> renderer;
        std::shared_ptr<SceneManager> scene_manager;
        std::shared_ptr<UI> ui;


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


        std::shared_ptr<FlyCameraControls> cameracontrols;
};


