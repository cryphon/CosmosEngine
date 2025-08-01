// Camera.cpp
#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

Camera::Camera(Transform transform, float fov, float aspect, float near, float far)
    : transform_(transform), fov_(fov), aspect_ratio_(aspect), near_(near), far_(far) {
    this->transform_.update_matrices();
}

void Camera::set_yaw(float yaw) {
    transform_.rotation.y = glm::mod(yaw, 360.0f);
    transform_.cache_trigger = true;
}

void Camera::set_pitch(float pitch) {
    constexpr float POLAR_CAP = 89.0f;
    transform_.rotation.x = glm::clamp(pitch, -POLAR_CAP, POLAR_CAP);
    transform_.cache_trigger = true;
}

void Camera::rotate(float yawOffset, float pitchOffset, bool constrainPitch) {
    float new_yaw = transform_.rotation.y + yawOffset;
    float new_pitch = transform_.rotation.x + pitchOffset;

    set_yaw(new_yaw);
    if (constrainPitch) set_pitch(new_pitch);
    else transform_.rotation.x = new_pitch;

    transform_.cache_trigger = true;
}

void Camera::move(CameraDirection direction, float velocity) {
    glm::vec3 forward = get_forward();
    glm::vec3 right = get_right();
    glm::vec3 up = get_up();

    switch (direction) {
        case CameraDirection::FORWARD:
            transform_.position += forward * velocity;
            break;
        case CameraDirection::BACKWARD:
            transform_.position -= forward * velocity;
            break;
        case CameraDirection::LEFT:
            transform_.position -= right * velocity;
            break;
        case CameraDirection::RIGHT:
            transform_.position += right * velocity;
            break;
        case CameraDirection::UP:
            transform_.position += up * velocity;
            break;
        case CameraDirection::DOWN:
            transform_.position -= up * velocity;
            break;
    }
    transform_.cache_trigger = true;
}

void Camera::zoom(float offset) {
    fov_ = glm::clamp(fov_ - offset, 1.0f, 135.0f);
}



glm::mat4 Camera::get_view_matrix() const {
    glm::vec3 eye = transform_.position;
    glm::vec3 center = eye + get_forward();
    glm::vec3 up = get_up();
    return glm::lookAt(eye, center, up);
}

glm::mat4 Camera::get_projection_transform() const {
    return glm::perspective(glm::radians(fov_), aspect_ratio_, near_, far_);
}

void Camera::update_uniforms(Shader& shader) {
    shader.set_mat4("view", get_view_matrix());
    shader.set_mat4("projection", get_projection_transform());
}

void Camera::look_at(glm::vec3 target) {
    glm::vec3 dir = glm::normalize(target - transform_.position);
    transform_.rotation.x = glm::degrees(asin(dir.y));
    transform_.rotation.y = glm::degrees(atan2(dir.x, dir.z));
    transform_.cache_trigger = true;
}

