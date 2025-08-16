// ==
// Standard Library
// ==
#include <cmath>

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

        glm::quat q_yaw   = glm::angleAxis(glm::radians(yaw_deg_delta),   glm::normalize(world_up_));
        glm::vec3 right   = glm::normalize(glm::rotate(orientation_, glm::vec3(1,0,0)));
        glm::quat q_pitch = glm::angleAxis(glm::radians(pitch_deg_delta), right);

        // Apply in yaw-then-pitch order (both premultiplied)
        orientation_ = glm::normalize(q_pitch * q_yaw * orientation_);

        // Push to Camera (no position change)
        apply_orientation_to_camera();

    }   else if (mode_ == MouseMode::Pan) {
        // Pan along screen axes: right and world-up
        glm::vec3 right = glm::normalize(glm::rotate(orientation_, glm::vec3(1,0,0)));
        glm::vec3 up_ref = glm::normalize(world_up_); // vertical pan aligned with gravity

        glm::vec3 delta = (-dx * right + dy * up_ref) * pan_speed_;
        camera->set_position(camera->get_position() + delta);
        // Orientation unchanged
    }
}

void QuaternionCameraControls::sync_from_camera() {
    if (!camera) return;

    // Build orientation from current yaw/pitch (degrees) so there is no snap on first drag
    float yaw_deg   = camera->get_yaw();
    float pitch_deg = camera->get_pitch();

    glm::quat q_yaw   = glm::angleAxis(glm::radians(yaw_deg),   glm::normalize(world_up_));
    glm::vec3 right   = glm::normalize(glm::rotate(q_yaw, glm::vec3(1,0,0)));
    glm::quat q_pitch = glm::angleAxis(glm::radians(pitch_deg), right);

    orientation_ = glm::normalize(q_pitch * q_yaw);

    // Ensure camera vectors are coherent with orientation
    apply_orientation_to_camera();
}

void QuaternionCameraControls::apply_orientation_to_camera() {
    // Base camera forward is -Z in your class (yaw=-90,pitch=0 -> front=(0,0,-1))
    glm::vec3 base_forward(0.0f, 0.0f, -1.0f);
    glm::vec3 fwd = glm::normalize(glm::rotate(orientation_, base_forward));

    // Update camera front and also backfill yaw/pitch for consistency
    camera->set_front(fwd);

    // Derive yaw/pitch to keep your getters sane (optional but convenient)
    float pitch = glm::degrees(std::asin(glm::clamp(fwd.y, -1.0f, 1.0f)));
    float yaw   = glm::degrees(std::atan2(fwd.z, fwd.x)); // matches your get_view_matrix convention
    camera->set_pitch(pitch);
    camera->set_yaw(yaw);
}
}