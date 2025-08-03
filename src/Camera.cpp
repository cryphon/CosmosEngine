#include "Camera.hpp"
#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>


Camera::Camera()
    : position_(0.0f, 0.0f, 3.0f),
      world_up_(0.0f, 1.0f, 0.0f),
      yaw_(-90.0f),
      pitch_(0.0f)
{
    update_vectors();
    update_view();
}


void Camera::update_view() {view_matrix = glm::lookAt(position_, position_ + front_, up_); }
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
    if (!rotating) return;

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
