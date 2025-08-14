#pragma once

#include "Camera.hpp"

class QuaternionCamera {
    public:
        void Rotate();
        glm::quat rotation (float angle, glm::vec3) {};
        glm::quat conjugate(float w, float x, float y, float z) {};
        
    private:

};

class QuaternionCameraControls : public CameraControls {
    public:
        QuaternionCameraControls(GLFWwindow* window, std::shared_ptr<Camera> camera, glm::vec3 target = glm::vec3(0.0f));

        void update(Camera& camera, float delta_t) override;
        void on_mouse_move(double xpos, double ypos) override;
        void on_mouse_button(int button, int action, int mods) override;
        void on_scroll(double xoffset, double yoffset) override;
        void set_angles(float new_yaw, float new_pitch);
        void set_passive_rotation(bool enabled) { passive_rotation_enabled_ = enabled; }
        void set_rotation_speed(float speed) { passive_rotation_speed_ = speed; }
        void apply_man_update();
        float get_yaw() const { return yaw; }
        float get_pitch() const { return pitch; }


    private:
        void update_camera(Camera& camera);

        GLFWwindow* window;
        std::shared_ptr<Camera> camera;
        glm::vec3 target;

        bool rotating = false;
        bool first_mouse = true;
        double last_x = 0.0, last_y = 0.0;

        float radius = 10.0f;
        float yaw = -90.0f;
        float pitch = 0.0f;

        float sensitivity = 0.3f;
        float zoom_speed = 0.5f;

        float passive_rotation_speed_ = 10.0f; // degrees per second
        bool passive_rotation_enabled_ = false;

};