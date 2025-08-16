// ==
// Standard Library
// ==

// ==
// Third Party
// ==
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// ==
// Cosmos
// ==
#include <cosmos/input/InputManager.hpp>
#include <cosmos/input/InputListener.hpp>
#include <cosmos/core/Logger.hpp>

namespace cosmos::input {

InputManager::InputManager(GLFWwindow* window)
    : window_(window) {
    if (!window) {
        LOG_ERROR("[InputManager] Constructor received null GLFWwindow*");
    }
    set_input_cbs();
}

void InputManager::set_input_cbs() {
    glfwSetWindowUserPointer(window_, this);

    glfwSetCursorPosCallback(window_, glfw_cursor_cb);
    glfwSetMouseButtonCallback(window_, glfw_mouse_button_cb);
    glfwSetKeyCallback(window_, glfw_key_cb);
    glfwSetScrollCallback(window_, glfw_scroll_cb);

    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void InputManager::add_listener(InputListener* listener) {
    listeners_.push_back(listener);
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

void InputManager::glfw_scroll_cb(GLFWwindow* win, double xoffset, double yoffset) {
    auto* self = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
    if (self) self->dispatch_scroll(xoffset, yoffset);
}

// Dispatcher methods
void InputManager::dispatch_mouse_move(double xpos, double ypos) {
    for (auto* listener : listeners_)
        listener->on_mouse_move(xpos, ypos);
}

void InputManager::dispatch_mouse_button(int button, int action, int mods) {
    for (auto* listener : listeners_)
        listener->on_mouse_button(button, action, mods);
}

void InputManager::dispatch_key(int key, int action, int mods) {
    for (auto* listener : listeners_)
        listener->on_key(key, action, mods);
}

void InputManager::dispatch_scroll(double xoffset, double yoffset) {
    for (auto* listener : listeners_) {
        listener->on_scroll(xoffset, yoffset);
    }
}

void InputManager::update(float dt) {
    for (auto* listener : listeners_)
        listener->update(dt);
}
}



