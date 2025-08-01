#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <memory>
#include <Shader.hpp>
#include "Window.hpp"
#include "Core.hpp"
#include "Transform.hpp"
#include "Logger.hpp"


enum class CameraDirection {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};



constexpr float DEFAULT_FOV = 45.0f;
constexpr float DEFAULT_NEAR = 0.1f;
constexpr float DEFAULT_FAR = 100.0f;
constexpr float DEFAULT_ASPECT_RATIO = 4.0f / 3.0f;

constexpr float MIN_FOV = 1.0f;
constexpr float MAX_FOV = 135.0f;


class Camera {
public:
    Camera(Transform transform, float fov = DEFAULT_FOV, float aspectRatio = DEFAULT_ASPECT_RATIO,
         float near = DEFAULT_NEAR, float far = DEFAULT_FAR);


    virtual ~Camera() = default;   

    void look_at(glm::vec3 center);

    glm::vec3 get_pos() const { return transform_.position; }
    void set_pos(glm::vec3 position) { transform_.position = position; }

    float get_fov() const { return fov_; }
    void set_fov(float fov) { fov_ = fov; }

    float get_aspect_ratio() const { return aspect_ratio_; }
    void set_aspect_ratio(float aspect_ratio) { 
        aspect_ratio_ = aspect_ratio; }
    void set_aspect_ratio(const ImageSize& size) {
        if (size.height > 0) {
            aspect_ratio_ = static_cast<float>(size.width) / size.height;
        } else {
            // fallback or warning
            LOG_INFO("Invalid window height: defaulting aspect ratio to 4:3");
            aspect_ratio_ = 4.0f / 3.0f;
        }
}

    glm::vec3 get_forward() const {
    glm::quat q = glm::quat(glm::radians(transform_.rotation));
    return glm::rotate(q, glm::vec3(0.0f, 0.0f, -1.0f));
}

    float get_near_plane() const { return near_; }
    void set_near_plane(float near) { near_ = near; }
    float get_far_plane() const { return far_; }
    void set_far_plane(float far) { far_ = far; }

    glm::mat4 get_view_matrix() const;
    glm::mat4 get_projection_transform() const;
    Transform get_transform() const { return transform_; }

    void update_uniforms(Shader& shader);

    void move(CameraDirection direction, float vel);
    void rotate(float yaw_offset, float pitch_offset, bool constrain_pitch = true);
    void zoom(float fov_offset);

    glm::vec3 get_position() const { return transform_.position; }

    glm::vec3 get_right() const {
        return glm::normalize(glm::cross(get_forward(), world_up_));
    }
glm::vec3 get_up() const {
    return glm::normalize(glm::cross(get_right(), get_forward()));
}

    float get_yaw() const { return transform_.rotation.y; }
    float get_pitch() const { return transform_.rotation.x; }

    void set_yaw(float yaw);
    void set_pitch(float pitch);


private:
    Transform transform_;
    glm::vec3 world_up_ = glm::vec3(0.0f, 1.0f, 0.0f);

    float fov_;
    float aspect_ratio_;
    float near_;
    float far_;
};

        
