class Engine;
#include "InputManager.hpp"
#include "imgui.h"
#include <iostream>

InputManager* get_input(GLFWwindow* window);  // from Engine.cpp

InputManager::InputManager(GLFWwindow* window, std::shared_ptr<PerspectiveCamera> camera)
    : window(window), camera(camera)
{
    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double xpos, double ypos) {
        if (auto* input = get_input(win)) {
            input->handle_mouse(xpos, ypos);
        }
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* win, int button, int action, int mods) {
        if (auto* input = get_input(win)) {
            input->handle_mouse_button(button, action, mods);
        }
    });

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}


void InputManager::handle_mouse(double xpos, double ypos) {
    if (!rotating) return;
    
    if (first_drag) {
        last_x = xpos;
        last_y = ypos;
        first_drag = false;
        return;
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

    camera->update_view();
}

void InputManager::handle_mouse_button(int button, int action, int mods) {
    if (ImGui::GetIO().WantCaptureMouse) return;

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            rotating = true;
            first_drag = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else if (action == GLFW_RELEASE) {
            rotating = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        handle_click(xpos, ypos);
    }
}

void InputManager::handle_click(double xpos, double ypos) {
    glm::vec3 ray = compute_mouse_ray(xpos, ypos);
    glm::vec3 origin = camera->get_position();

    int hit_id = raycast_scene(origin, ray);

    if (hit_id == -1) {
        selected_object_id = -1; // Deselect
    } else if (selected_object_id == hit_id) {
        selected_object_id = -1; // Toggle deselect
    } else {
        selected_object_id = hit_id; // Select new
    }
}

glm::vec3 InputManager::compute_mouse_ray(double mouse_x, double mouse_y) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    
    glm::mat4 projection_matrix = camera->get_projection_matrix();
    glm::mat4 view_matrix = camera->get_view_matrix();



    float x = (2.0f * mouse_x) / width - 1.0f;
    float y = 1.0f - (2.0f * mouse_y) / height;
    float z = 1.0f;

    // calc normalized device coordinates
    glm::vec3 ray_nds = glm::vec3(x, y, z);

    // homogeneous clip coordinates
    glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);

    // eye camera coordinates
    glm::vec4 ray_eye = inverse(projection_matrix) * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

    // world coordinates
    glm::vec3 ray_world = (inverse(view_matrix) * ray_eye);
    ray_world = glm::normalize(ray_world);

    return ray_world;
}

int InputManager::raycast_scene(const glm::vec3& origin, const glm::vec3& ray) {
    // Plane at y = 0 (horizontal ground)
    glm::vec3 plane_point(0.0f, 0.0f, 0.0f); // A point on the plane
    glm::vec3 plane_normal(0.0f, 1.0f, 0.0f); // Upward facing

    float denom = glm::dot(ray, plane_normal);
    if (fabs(denom) < 1e-6f) {
        // Ray is parallel to plane
        return -1;
    }

    float t = glm::dot(plane_point - origin, plane_normal) / denom;
    if (t < 0.0f) {
        // Intersection is behind the camera
        return -1;
    }

    glm::vec3 hit_point = origin + ray * t;

    return 0; 
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
    camera->update_view();
}

