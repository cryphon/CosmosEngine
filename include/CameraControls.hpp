#pragma once
#include "Camera.hpp"
#include <memory>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

struct MouseDelta {
  float xoffset;
  float yoffset;
};


class CameraControls {
    public:
        virtual void resize_window(int width, int height) = 0;
        virtual void scroll(Camera& camera, double xoffset, double yoffset,
                bool mouse_captured) = 0;
        virtual void mouse_move(Camera& camera, double xpos, double ypos,
                bool mouse_captured) = 0;
        virtual void mouse_button(Camera& camera, int button, int action, int mods,
                bool mouse_captured) = 0;
        virtual void process_input(GLFWwindow* window, Camera& camera,
                float delta_time) = 0;

        // Base helpers; call these from subclasses.
        void handle_dragstartend(int button, int action);
        MouseDelta calculate_mousedelta(double xpos, double ypos);

        float get_speed() const { return speed_; }
        void set_speed(float speed) { speed_ = speed; }
        float get_sensitivity() const { return sensitivity_; }
        void set_sensitivity(float sensitivity) { sensitivity_ = sensitivity; }

    protected:
        static constexpr float DEFAULT_SPEED = 5.0f;
        static constexpr float DEFAULT_SENSITIVITY = 0.1f;

        // Whether input updates should use the last mouse positions to calculate
        // delta, or to start from the current positions.
        bool initialized_ = false;
        // Whether a mouse is being click+dragged.
        bool dragging_ = false;
        float last_mouse_x_;
        float last_mouse_y_;

        // General setting for camera movement speed.
        float speed_ = DEFAULT_SPEED;
        // General setting for camera sensitivity.
        float sensitivity_ = DEFAULT_SENSITIVITY;
};

class FlyCameraControls : public CameraControls {
    public:
        virtual ~FlyCameraControls() = default;
        void resize_window(int width, int height) override;
        void scroll(Camera& camera, double xoffset, double yoffset,
                bool mouse_captured) override;
        void mouse_move(Camera& camera, double xpos, double ypos,
                bool mouse_captured) override;
        void mouse_button(Camera& camera, int button, int action, int mods,
                bool mouse_captured) override;
        void process_input(GLFWwindow* window, Camera& camera,
                float delta_time) override;
};


class OrbitCameraControls : public CameraControls {
    public:
        OrbitCameraControls(Camera& camera, glm::vec3 center = glm::vec3(0.0f));
        virtual ~OrbitCameraControls() = default;
        void resize_window(int width, int height) override;
        void scroll(Camera& camera, double xoffset, double yoffset,
                bool mouse_captured) override;
        void mouse_move(Camera& camera, double xpos, double ypos,
                bool mouse_captured) override;
        void mouse_button(Camera& camera, int button, int action, int mods,
                bool mouse_captured) override;
        void process_input(GLFWwindow* window, Camera& camera,
                float delta_time) override;

        glm::vec3 get_center() const { return center_; }
        void set_center(glm::vec3 center) { center_ = center; }

        // Updates the camera position and orientation to match the current orbit
        // position.
        void update_camera(Camera& camera);

    private:
        static constexpr float MIN_RADIUS = 0.1f;
        static constexpr float MAX_RADIUS = 100.0f;
        // The center point that we're orbiting around.
        glm::vec3 center_;
        // The distance from the center.
        float radius_;
        // The **clockwise** angle in degrees that the camera is rotated about the
        // center.
        float azimuth_;
        // The angle in degrees that the camera is tilted up or down.
        float altitude_;
};
