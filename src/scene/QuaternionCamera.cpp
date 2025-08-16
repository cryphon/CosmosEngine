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
#include <cosmos/scene/QuaternionCamera.hpp>

namespace cosmos::scene {
    
QuaternionCameraControls::QuaternionCameraControls(GLFWwindow* window, std::shared_ptr<Camera> camera, glm::vec3 target)
    : window(window), camera(camera), target(target) 
{
    position_ = this->camera ? this->camera->get_position()
                             : (target - glm::vec3(0,0,-1) * radius);
}

void QuaternionCameraControls::on_mouse_button(int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        first_mouse = true;
        dragging = true;
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            mode_ = MouseMode::FreeLook;
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            mode_ = MouseMode::Pan;
        }
    } else if (action == GLFW_RELEASE) {
        dragging = false;
        mode_ = MouseMode::None;
    }
}

void QuaternionCameraControls::on_mouse_move(double xpos, double ypos) {
    if (!dragging) return;

    if (first_mouse) {
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
        return;
    }

    float dx = static_cast<float>(xpos - last_x);
    float dy = static_cast<float>(last_y - ypos);
    last_x = xpos; last_y = ypos;

    dx *= sensitivity; 
    dy *= sensitivity; 

   
}

void QuaternionCameraControls::update(Camera& cam, float delta_t) {
    update_camera(cam);
}

void QuaternionCameraControls::on_scroll(double xoffset, double yoffset) {
    radius -= static_cast<float>(yoffset) * zoom_speed;
    radius = glm::clamp(radius, 1.0f, 200.0f);
}


void QuaternionCameraControls::update_camera(Camera& cam) {
    float yaw_rad = glm::radians(yaw_deg_);
    float pitch_rad = glm::radians(pitch_deg_);

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
}