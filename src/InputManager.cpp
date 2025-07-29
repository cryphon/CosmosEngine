class Engine;
#include "InputManager.hpp"
#include "imgui.h"
#include <iostream>
#include "RenderableScene.hpp"
#include "SceneManager.hpp"
#include "SceneObject.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "ShaderLibrary.hpp"
#include "Renderer.hpp"

InputManager* get_input(GLFWwindow* window);  // from Engine.cpp



InputManager::InputManager(GLFWwindow* window, std::shared_ptr<PerspectiveCamera> camera, std::shared_ptr<SceneManager> scene_manager, std::shared_ptr<Renderer> renderer)
    : window(window), camera(camera), scene_manager(scene_manager), renderer(renderer)
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

    RenderableScene* scene = static_cast<RenderableScene*>(scene_manager->get_current_scene_obj());
    auto objects = scene->get_objects();
    int hit_id = ray_intersects_object(origin, ray, objects);

    if (hit_id == -1 || hit_id == selected_object_id) {
        // No hit or deselecting same object
        renderer->set_selected_object(-1);
        selected_object_id = -1;
    } else {
        // New selection
        renderer->set_selected_object(hit_id);
        selected_object_id = hit_id;
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

int InputManager::ray_intersects_object(const glm::vec3& origin, const glm::vec3& ray, const std::vector<SceneObject>& objects) {
    float closest_distance = std::numeric_limits<float>::max();
    int hit_id = -1;

    for (const auto& obj : objects) {
        auto mesh = obj.mesh;
        if (!mesh) continue;

        const glm::mat4& model = obj.transform.model_matrix;
        const std::vector<glm::vec3>& vertices = mesh->get_vertices();
        const std::vector<unsigned int>& indices = mesh->get_indices();     

        for (size_t i = 0; i < indices.size(); i += 3) {
            glm::vec3 v0 = glm::vec3(model * glm::vec4(vertices[indices[i]],     1.0));
            glm::vec3 v1 = glm::vec3(model * glm::vec4(vertices[indices[i + 1]], 1.0));
            glm::vec3 v2 = glm::vec3(model * glm::vec4(vertices[indices[i + 2]], 1.0));

            float t;
            if (intersect_ray_triangle(origin, ray, v0, v1, v2, t)) {
                if (t < closest_distance) {
                    closest_distance = t;
                    hit_id = obj.get_id();
                }
            }
        }
    }

    return hit_id;
}

bool InputManager::intersect_ray_triangle(const glm::vec3& origin, const glm::vec3& dir,
                            const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
                            float& t_out)
{
    const float EPSILON = 1e-6f;
    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;
    glm::vec3 h = glm::cross(dir, edge2);
    float a = glm::dot(edge1, h);

    if (a > -EPSILON && a < EPSILON)
        return false; // Ray is parallel to triangle

    float f = 1.0f / a;
    glm::vec3 s = origin - v0;
    float u = f * glm::dot(s, h);
    if (u < 0.0f || u > 1.0f)
        return false;

    glm::vec3 q = glm::cross(s, edge1);
    float v = f * glm::dot(dir, q);
    if (v < 0.0f || u + v > 1.0f)
        return false;

    float t = f * glm::dot(edge2, q);
    if (t > EPSILON) {
        t_out = t;
        return true;
    }

    return false;
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

