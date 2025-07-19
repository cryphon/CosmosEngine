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

    camera->yaw   += xoffset * sensitivity;
    camera->pitch += yoffset * sensitivity;

    if (camera->pitch > 89.0f)  camera->pitch = 89.0f;
    if (camera->pitch < -89.0f) camera->pitch = -89.0f;

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

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->position += camera->front * speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->position -= camera->front * speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->position -= camera->right * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->position += camera->right * speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->position -= camera->up * speed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->position += camera->up * speed;
}
