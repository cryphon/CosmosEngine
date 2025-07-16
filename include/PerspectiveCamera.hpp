#ifndef PERSPECTIVE_CAMERA_HPP
#define PERSPECTIVE_CAMERA_HPP

#include "Camera.hpp"

class PerspectiveCamera : public Camera {
public:
    glm::vec3 position;
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up    = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 target;

    float yaw   = -90.0f;  // facing -Z
    float pitch = 0.0f;
    float fov_deg = 60.0f;
    float near_plane = 0.1f;
    float far_plane = 100.0f;

    PerspectiveCamera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f));

    PerspectiveCamera(glm::vec3 pos, glm::vec3 target, glm::vec3 up);


    void update_direction();
    glm::mat4 get_view_matrix() const override;

    glm::mat4 get_projection_matrix(int width, int height) const override;
};

#endif

