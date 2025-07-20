#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>
#include "PerspectiveCamera.hpp"


class InputManager {
    public:
        InputManager(GLFWwindow*, std::shared_ptr<PerspectiveCamera> camera);
        void update(float dt);
        void cursor_pos_callback(GLFWwindow* window, double pos_x, double pos_y);
        void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

    private:
        GLFWwindow* window;
        std::shared_ptr<PerspectiveCamera> camera;

        double last_x = 400.0, last_y = 400.0;
        bool first_drag = true;
        bool rotating = false;
        bool selecting = false;
        int selected_object_id = -1;
        float sensitivity = 0.1f;

        void handle_mouse(double pos_x, double pos_y);
        void handle_mouse_button(int button, int action, int mods);
        void handle_click(double xpos, double ypos);
        
        glm::vec3 compute_mouse_ray(double xpos, double ypos);
        int raycast_scene(const glm::vec3& origin, const glm::vec3& ray);
};

