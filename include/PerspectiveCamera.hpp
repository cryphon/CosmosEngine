#ifndef PERSPECTIVE_CAMERA_HPP
#define PERSPECTIVE_CAMERA_HPP

#include "Camera.hpp"

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f));

    PerspectiveCamera(glm::vec3 pos, glm::vec3 target, glm::vec3 up);


    void update_direction();
    
    // Getters
    glm::vec3 get_position() const override { return position; }
    glm::vec3 get_front() const { return front; }
    glm::vec3 get_up() const { return up; }
    glm::vec3 get_right() const { return right; }
    glm::vec3 get_target() const { return target; }

    float get_yaw() const { return yaw; }
    float get_pitch() const { return pitch; }
    float get_fov() const { return fov_deg; }
    float get_near_plane() const { return near_plane; }
    float get_far_plane() const { return far_plane; }

    // Setters
    void set_position(const glm::vec3& pos) { position = pos; }
    void set_front(const glm::vec3& f) { front = f; }
    void set_up(const glm::vec3& u) { up = u; }
    void set_right(const glm::vec3& r) { right = r; }
    void set_target(const glm::vec3& t) { target = t; }

    void set_yaw(float y) { yaw = y; }
    void set_pitch(float p) { pitch = p; }
    void set_fov(float fov) { fov_deg = fov; }
    void set_near_plane(float near) { near_plane = near; }
    void set_far_plane(float far) { far_plane = far; }

    glm::mat4 get_view_matrix() const override;
    glm::mat4 get_projection_matrix(int width, int height) const override;

private:
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


};

#endif

