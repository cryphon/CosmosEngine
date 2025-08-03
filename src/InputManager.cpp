class Engine;
#include "InputManager.hpp"
#include "InputListener.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Logger.hpp"

InputManager::InputManager(GLFWwindow* window)
    : window(window) {
    if (!window) {
        LOG_ERROR("[InputManager] Constructor received null GLFWwindow*");
    }
    set_input_cbs();
}

void InputManager::set_input_cbs() {
    glfwSetWindowUserPointer(window, this);

    glfwSetCursorPosCallback(window, glfw_cursor_cb);
    glfwSetMouseButtonCallback(window, glfw_mouse_button_cb);
    glfwSetKeyCallback(window, glfw_key_cb);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void InputManager::add_listener(InputListener* listener) {
    listeners.push_back(listener);
}

// Static -> Instance relay
void InputManager::glfw_cursor_cb(GLFWwindow* win, double xpos, double ypos) {
    auto* self = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
    if (self) self->dispatch_mouse_move(xpos, ypos);
}

void InputManager::glfw_mouse_button_cb(GLFWwindow* win, int button, int action, int mods) {
    auto* self = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
    if (self) self->dispatch_mouse_button(button, action, mods);
}

void InputManager::glfw_key_cb(GLFWwindow* win, int key, int scancode, int action, int mods) {
    auto* self = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
    if (self) self->dispatch_key(key, action, mods);
}

// Dispatcher methods
void InputManager::dispatch_mouse_move(double xpos, double ypos) {
    for (auto* listener : listeners)
        listener->on_mouse_move(xpos, ypos);
}

void InputManager::dispatch_mouse_button(int button, int action, int mods) {
    for (auto* listener : listeners)
        listener->on_mouse_button(button, action, mods);
}

void InputManager::dispatch_key(int key, int action, int mods) {
    for (auto* listener : listeners)
        listener->on_key(key, action, mods);
}

void InputManager::update(float dt) {
    for (auto* listener : listeners)
        listener->update(dt);
}

