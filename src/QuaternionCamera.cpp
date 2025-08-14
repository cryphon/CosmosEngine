#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "QuaternionCamera.hpp"

QuaternionCameraControls::QuaternionCameraControls(GLFWwindow* window, std::shared_ptr<Camera> camera, glm::vec3 target)
    : window(window), camera(camera), target(target) {}

void OrbitalCameraControls::on_mouse_button(int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            rotating = true;
            first_mouse = true;
        } else if (action == GLFW_RELEASE) {
            rotating = false;
        }
    }
}

void QuaternionCameraControls::on_mouse_move(double xpos, double ypos) {
    if (!rotating) return;

    if (first_mouse) {
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
        return;
    }

    float x_offset = static_cast<float>(xpos - last_x);
    float y_offset = static_cast<float>(last_y - ypos); // reversed: y ranges bottom to top

    last_x = xpos;
    last_y = ypos;

    x_offset *= sensitivity;
    y_offset *= sensitivity;

    yaw += x_offset;
    pitch += y_offset;

    // clamp pitch to prevent flipping
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}

void QuaternionCameraControls::update(Camera& cam, float delta_t) {
    if (passive_rotation_enabled_) {
        yaw += passive_rotation_speed_ * delta_t;
        if (yaw > 360.0f) yaw -= 360.0f;
    }

    update_camera(cam);
}

void QuaternionCameraControls::on_scroll(double xoffset, double yoffset) {
    radius -= static_cast<float>(yoffset) * zoom_speed;
    radius = glm::clamp(radius, 1.0f, 200.0f);
}


void QuaternionCameraControls::update_camera(Camera& cam) {
    float yaw_rad = glm::radians(yaw);
    float pitch_rad = glm::radians(pitch);

    // Compute new position
    glm::vec3 offset;
    offset.x = radius * cos(pitch_rad) * sin(yaw_rad);
    offset.y = radius * sin(pitch_rad);
    offset.z = radius * cos(pitch_rad) * cos(yaw_rad);

    glm::vec3 position = target + offset;
    glm::vec3 front = glm::normalize(target - position);

    float computed_yaw = glm::degrees(atan2(front.z, front.x));
    float computed_pitch = glm::degrees(asin(front.y));

    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 up    = glm::normalize(glm::cross(right, front));

    cam.set_manual_orientation(true); // disable update_vectors
    cam.set_position(position);
    cam.set_front(front);
    cam.set_right(right);
    cam.set_up(up);

    cam.set_yaw(computed_yaw);     
    cam.set_pitch(computed_pitch);

    cam.update_view();
}

void Rotate(float angle, glm::vec3 vectors) {
    
}

glm::quat rotation (float angle, glm::vec3) {

}

glm::quat conjugate(float w, float x, float y, float z) {
    return glm::quat{w, -x, -y, -z};
}

void QuaternionCameraControls::set_angles(float new_yaw, float new_pitch) {
    yaw = new_yaw;
    pitch = glm::clamp(new_pitch, -89.0f, 89.0f); // prevent flip
}

void QuaternionCameraControls::apply_man_update() {
    update_camera(*camera);
}
