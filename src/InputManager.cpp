#include "InputManager.hpp"
#include "imgui.h"

static InputManager* get_input(GLFWwindow* window) {
    return static_cast<InputManager*>(glfwGetWindowUserPointer(window));
}

InputManager::InputManager(GLFWwindow* window, std::shared_ptr<PerspectiveCamera> camera)
    : window(window), camera(camera)
{
    glfwSetWindowUserPointer(window, this);

    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double xpos, double ypos) {
        auto* input = get_input(win);
        if (input) input->handle_mouse(xpos, ypos);
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* win, int button, int action, int mods) {
        auto* input = get_input(win);
        if (input) input->handle_mouse_button(button, action, mods);
    });

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}


void InputManager::handle_mouse(double xpos, double ypos) {
    if (!rotating) {
        last_x = xpos;
        last_y = ypos;
        return;
    }

    if (first_drag) {
        last_x = xpos;
        last_y = ypos;
        first_drag = false;
    }

    float xoffset = float(xpos - last_x);
    float yoffset = float(last_y - ypos);

    last_x = xpos;
    last_y = ypos;

    float yaw = camera->get_yaw();
    float pitch = camera->get_pitch();

    camera->set_yaw(yaw + xoffset * sensitivity);
    camera->set_pitch(pitch + yoffset * sensitivity);

    if (camera->get_pitch() > 89.0f)  camera->set_pitch(89.0f);
    if (camera->get_pitch() < -89.0f) camera->set_pitch(-89.0f);

    camera->update_direction();
}

void InputManager::handle_mouse_button(int button, int action, int mods) {
    if (ImGui::GetIO().WantCaptureMouse) return;

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            rotating = true;
            first_drag = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else if (action == GLFW_RELEASE) {
            rotating = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void InputManager::update(float delta_time) {
    float speed = 5.0f * delta_time;

    glm::vec3 pos = camera->get_position();
    glm::vec3 front = camera->get_front();
    glm::vec3 right = camera->get_right();
    glm::vec3 up = camera->get_up();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        pos += front * speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        pos -= front * speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        pos -= right * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        pos += right * speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        pos -= up * speed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        pos += up * speed;

    camera->set_position(pos);
}

