#include "PerspectiveCamera.hpp"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include "Logger.hpp"



std::string format_mat4(const glm::mat4& m) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);  // Optional: format numbers
    for (int row = 0; row < 4; ++row) {
        oss << "[ ";
        for (int col = 0; col < 4; ++col) {
            oss << std::setw(6) << m[col][row] << " ";  // Note: GLM is column-major
        }
        oss << "]\n";
    }
    return oss.str();
}

PerspectiveCamera::PerspectiveCamera(glm::vec3 pos) : position(pos) {
    update_view();
}

PerspectiveCamera::PerspectiveCamera(glm::vec3 pos, glm::vec3 tgt, glm::vec3 up_dir)
    : position(pos), target(tgt), up(up_dir) {
    update_view();
}

void PerspectiveCamera::update_view() {
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);

    // Recalculate right and up vectors
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::normalize(glm::cross(right, front));

    // Recalculate view matrix
    view = glm::lookAt(position, position + front, up);
}


void PerspectiveCamera::update_projection() {
    projection = glm::perspective(glm::radians(fov_deg), aspect_ratio, near_plane, far_plane);
    glm::mat4 projection = glm::perspective(glm::radians(fov_deg), aspect_ratio, near_plane, far_plane);
    LOG_INFO("Projection matrix:\n" + format_mat4(projection));
}


void PerspectiveCamera::reset_camera() {
    position = glm::vec3(0.0f, 1.0f, 3.0f);
    target = glm::vec3(0.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    update_view();
};
