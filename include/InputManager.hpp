#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>
#include "PerspectiveCamera.hpp"


class InputManager {
    public:
        InputManager(GLFWwindow*, std::shared_ptr<PerspectiveCamera> camera);
        void update(float dt);


    private:
        GLFWwindow* window;
        std::shared_ptr<PerspectiveCamera> camera;


        double last_x = 400.0, last_y = 400.0;
        bool first_drag = true;
        bool rotating = false;
        float sensitivity = 0.01f;

        void handle_mouse(double pos_x, double pos_y);
        void handle_mouse_button(int button, int action, int mods);

        friend void cursor_pos_callback(GLFWwindow* window, double pos_x, double pos_y);
        friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};

