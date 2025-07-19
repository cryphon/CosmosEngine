#include "PerspectiveCamera.hpp"
#include <iostream>


PerspectiveCamera::PerspectiveCamera(glm::vec3 pos) : position(pos) {
    update_direction();
}

PerspectiveCamera::PerspectiveCamera(glm::vec3 pos, glm::vec3 tgt, glm::vec3 up_dir)
    : position(pos), target(tgt), up(up_dir) {
    update_direction();
}

void PerspectiveCamera::update_direction() {
    glm::vec3 dir;
    dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir.y = sin(glm::radians(pitch));
    dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(dir);
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    up    = glm::normalize(glm::cross(right, front));
}

glm::mat4 PerspectiveCamera::get_view_matrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 PerspectiveCamera::get_projection_matrix(int width, int height) const {
    return glm::perspective(glm::radians(fov_deg), (float)width / height, near_plane, far_plane);
}

void PerspectiveCamera::reset_camera() {
    position = glm::vec3(0.0f, 1.0f, 3.0f);
    target = glm::vec3(0.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    update_direction();
};
