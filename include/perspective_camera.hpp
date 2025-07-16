#ifndef PERSPECTIVE_CAMERA_HPP
#define PERSPECTIVE_CAMERA_HPP

#include "camera.hpp"

class PerspectiveCamera : public Camera {
public:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;

    float fov_deg = 60.0f;
    float near_plane = 0.1f;
    float far_plane = 100.0f;

    PerspectiveCamera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3 target = glm::vec3(0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)
    ) : position(position), target(target), up(up) {}

    glm::mat4 get_view_matrix() const override {
        return glm::lookAt(position, target, up);
    }

    glm::mat4 get_projection_matrix(int width, int height) const override {
        return glm::perspective(glm::radians(fov_deg), static_cast<float>(width) / height, near_plane, far_plane);
    }
};

#endif

