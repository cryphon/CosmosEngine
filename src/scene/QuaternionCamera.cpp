// ==
// Standard Library
// ==
#include <cmath>
#include <algorithm>

// ==
// Third Party
// ==
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


// ==
// Cosmos
// ==   
#include <cosmos/scene/Camera.hpp>
#include <cosmos/scene/QuaternionCamera.hpp>

namespace cosmos::scene {
    
static inline float clamp_pitch_deg(float deg) {
    const float limit = 89.0f;
    return std::clamp(deg, -limit, limit);
}

static inline glm::quat compose_yaw_pitch(const glm::vec3& world_up, float yaw_deg, float pitch_deg)
{
    const glm::vec3 up = glm::normalize(world_up);
    glm::quat q_yaw   = glm::angleAxis(glm::radians(yaw_deg), up);
    glm::vec3 right   = glm::normalize(glm::rotate(q_yaw, glm::vec3(1,0,0)));
    glm::quat q_pitch = glm::angleAxis(glm::radians(pitch_deg), right);
    return glm::normalize(q_pitch * q_yaw);
}

QuaternionCameraControls::QuaternionCameraControls(GLFWwindow* window, std::shared_ptr<Camera> camera, glm::vec3 target)
    : window(window), camera(camera), target(target) {}

void QuaternionCameraControls::on_mouse_button(int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        first_mouse = true;
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            mode_ = MouseMode::FreeLook;
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            mode_ = MouseMode::Pan;
        }
    } else if (action == GLFW_RELEASE) {
        mode_ = MouseMode::None;
    }
}

void QuaternionCameraControls::on_mouse_move(double xpos, double ypos) {
    if (mode_ == MouseMode::None) return;

    if (first_mouse) {
        last_x = xpos;
        last_y = ypos;

        first_mouse = false;
        return;
    }

    float dx = static_cast<float>(xpos - last_x);
    float dy = static_cast<float>(last_y - ypos);

    last_x = xpos; 
    last_y = ypos;

    if (mode_ == MouseMode::FreeLook) {
        float yaw_deg_delta   = dx * sensitivity_deg_per_px_;
        float pitch_deg_delta = dy * sensitivity_deg_per_px_;

        yaw_deg_   += yaw_deg_delta;
        pitch_deg_  = clamp_pitch_deg(pitch_deg_ + pitch_deg_delta);

        apply_orientation_to_camera(*this);

    }   else if (mode_ == MouseMode::Pan) {
        scaled_pan(dx, dy);
    }
}

void QuaternionCameraControls::on_scroll(double xoffset, double yoffset) {
    const float min_r = 0.05f, max_r = 1e6f;
    radius = std::clamp(radius - static_cast<float>(yoffset) * zoom_speed, min_r, max_r);
    apply_orientation_to_camera(*this);
}

void QuaternionCameraControls::scaled_pan(float dx, float dy) {
    glm::vec3 right  = glm::normalize(glm::rotate(orientation_, glm::vec3(1,0,0)));
    glm::vec3 up_ref = glm::normalize(world_up_);
    float scale      = pan_speed_ * std::max(radius, 0.001f);

    glm::vec3 delta = (-dx * right - dy * up_ref) * scale;
    target += delta;
    apply_orientation_to_camera(*this);
}


void QuaternionCameraControls::apply_orientation_to_camera(QuaternionCameraControls& self) {
    glm::quat q = compose_yaw_pitch(self.world_up_, self.yaw_deg_, self.pitch_deg_);

    // Rotate a base offset of length 'radius' to get camera position
    glm::vec3 base_offset(0.0f, 0.0f, radius);
    glm::vec3 offset = glm::rotate(q, base_offset);
    glm::vec3 pos    = target + offset;

    // Look at target
    glm::vec3 fwd = glm::normalize(target - pos);
    orientation_   = glm::quatLookAt(fwd, glm::normalize(world_up_));

    camera->set_position(pos);
    camera->set_front(fwd);

    // Backfill yaw/pitch to keep your Camera’s Euler view coherent
    float pitch = glm::degrees(std::asin(glm::clamp(fwd.y, -1.0f, 1.0f)));
    float yaw   = glm::degrees(std::atan2(fwd.z, fwd.x));
    camera->set_pitch(pitch);
    camera->set_yaw(yaw);
}
}