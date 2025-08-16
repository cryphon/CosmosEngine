// ==
// Standard Library
// ==

// ==
// Third Party
// ==
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <imgui.h>
#include <glm/gtx/string_cast.hpp>

// ==
// Cosmos
// ==
#include <cosmos/scene/Camera.hpp>


namespace cosmos::scene {
Camera::Camera()
    : position_(0.0f, 0.0f, 3.0f),
      world_up_(0.0f, 1.0f, 0.0f),
      yaw_(-90.0f),
      pitch_(0.0f)
{
    update_vectors();
    update_view();
}


void Camera::update_view() {
    if (!use_manual_orientation_) {
        update_vectors(); // yaw/pitch → front/right/up
    }
    view_matrix = glm::lookAt(position_, position_ + front_, up_);
}

void Camera::update_vectors() {
    // Recalculate front from yaw and pitch
    glm::vec3 f;
    f.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    f.y = sin(glm::radians(pitch_));
    f.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(f);

    // Recalculate right and up
    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_    = glm::normalize(glm::cross(right_, front_));
}

// Force vtable generation
CameraControls::~CameraControls() = default;
void CameraControls::on_mouse_move(double, double) {}
void CameraControls::on_mouse_button(int, int, int) {}
void CameraControls::on_scroll(double, double) {}




FlyCameraControls::FlyCameraControls(GLFWwindow* win, std::shared_ptr<Camera> cam)
    : window(win), camera(cam)
{
    if (camera) {
        yaw = camera->get_yaw();
        pitch = camera->get_pitch();
    }
}

void FlyCameraControls::update(Camera& camera, float delta_t) {
    this->update(delta_t);
}

void FlyCameraControls::update(float deltaTime) {
    glm::vec3 pos = camera->get_position();
    glm::vec3 front = camera->get_front();
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
    glm::vec3 up = glm::normalize(glm::cross(right, front));

    float velocity = speed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        pos += front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        pos -= front * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        pos -= right * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        pos += right * velocity;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        pos += up * velocity;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        pos -= up * velocity;

    camera->set_position(pos);
    camera->update_view();
}

void FlyCameraControls::update_camera_vectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    camera->set_yaw(yaw);
    camera->set_pitch(pitch);
    camera->set_front(glm::normalize(front));
    camera->update_view();
}

void FlyCameraControls::on_mouse_button(int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            rotating = true;
            first_mouse = true; // to prevent a jump
        } else if (action == GLFW_RELEASE) {
            rotating = false;
        }
    }
}

void FlyCameraControls::on_mouse_move(double xpos, double ypos) {
    if (!rotating || ImGui::GetIO().WantCaptureMouse) return;
    if (first_mouse) {
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
        return;
    }

    float xoffset = static_cast<float>(xpos - last_x);
    float yoffset = static_cast<float>(last_y - ypos); // reversed

    last_x = xpos;
    last_y = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // clamp pitch to prevent flip
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    update_camera_vectors();
}





OrbitalCameraControls::OrbitalCameraControls(GLFWwindow* window, std::shared_ptr<Camera> camera, glm::vec3 target)
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

void OrbitalCameraControls::on_mouse_move(double xpos, double ypos) {
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

void OrbitalCameraControls::update(Camera& cam, float delta_t) {
    if (passive_rotation_enabled_) {
        yaw += passive_rotation_speed_ * delta_t;
        if (yaw > 360.0f) yaw -= 360.0f;
    }

    update_camera(cam);
}

void OrbitalCameraControls::on_scroll(double xoffset, double yoffset) {
    radius -= static_cast<float>(yoffset) * zoom_speed;
    radius = glm::clamp(radius, 1.0f, 200.0f);
}


void OrbitalCameraControls::update_camera(Camera& cam) {
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

void OrbitalCameraControls::set_angles(float new_yaw, float new_pitch) {
    yaw = new_yaw;
    pitch = glm::clamp(new_pitch, -89.0f, 89.0f); // prevent flip
}

void OrbitalCameraControls::apply_man_update() {
    update_camera(*camera);
}
}
