#pragma once
// ==
// Standard Library
// ==

// ==
// Third Party
// ==
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

// ==
// Cosmos
// ==
#include <cosmos/scene/Camera.hpp>

namespace cosmos::scene {
    
class QuaternionCameraControls : public CameraControls {
    public:
        enum class MouseMode { None, FreeLook, Pan };

        QuaternionCameraControls(GLFWwindow* window, std::shared_ptr<Camera> camera, glm::vec3 target = glm::vec3(0.0f));

        void update(Camera& camera, float delta_t) override;
        
        void on_mouse_move(double xpos, double ypos) override;
        void on_mouse_button(int button, int action, int mods) override;
        void on_scroll(double xoffset, double yoffset) override;

    private:
        void update_camera(Camera& camera);

        GLFWwindow* window;
        std::shared_ptr<Camera> camera;
        glm::vec3 target;
        glm::quat orientation_ = glm::quat(1, 0, 0, 0);
        glm::vec3 position_;

        MouseMode mode_ = MouseMode::None;
        bool dragging = false;
        bool first_mouse = true;
        double last_x = 0.0, last_y = 0.0;

        float pitch_deg_ = 0.0f;
        float yaw_deg_ = 0.0f;

        float sensitivity = 0.3f;
        float zoom_speed = 0.5f;
        float radius = 10.0f;
};
}