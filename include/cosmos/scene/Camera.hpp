#pragma once
#include <memory>
// ==
// Standard Library
// ==
#include <memory>

// ==
// Third Party
// ==
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

// ==
// Cosmos
// ==
#include <cosmos/input/InputListener.hpp>


namespace cosmos::scene {

class Camera : public input::InputListener {
    public:
        Camera();
        glm::mat4 get_view_matrix() const {
            glm::vec3 front;
            front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
            front.y = sin(glm::radians(pitch_));
            front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
            return glm::lookAt(position_, position_ + glm::normalize(front), glm::vec3(0, 1, 0));
        }

        void update_projection() {
            projection_matrix = glm::perspective(glm::radians(fov_), aspect_, near_plane_, far_plane_);
        }

        glm::mat4 get_projection_matrix() const {
            return projection_matrix;
        }

        glm::vec3 get_position() const { return position_; }
        glm::vec3 get_front() const { return front_; }
        glm::vec3 get_right() const { return right_; }
        glm::vec3 get_up() const { return up_; }

        float get_pitch() const { return pitch_; }
        float get_yaw() const { return yaw_; }
        float get_fov() const { return fov_; }
        float get_aspect_ratio() const { return aspect_; }
        float get_near_plane() const { return near_plane_; }
        float get_far_plane() const { return far_plane_; }

        void set_position(const glm::vec3& pos) { position_ = pos; update_view(); }
        void set_front(const glm::vec3& f) { front_ = glm::normalize(f); update_vectors(); update_view(); }
        void set_right(const glm::vec3& r) { right_ = r; }
        void set_up(const glm::vec3& u) { up_ = u; }
        void set_pitch(float p) { pitch_ = p; }
        void set_yaw(float y) { yaw_ = y; }
        void set_fov(float f) { fov_ = f; }
        void set_aspect_ratio(float a) { aspect_ = a; }
        void set_near_plane(float n) { near_plane_ = n; }
        void set_far_plane(float f) { far_plane_ = f; }

        void update_view();
        void update_vectors();

        void set_manual_orientation(bool stat) { use_manual_orientation_ = stat; }

    private:
        glm::vec3 position_;
        glm::vec3 front_;
        glm::vec3 up_;
        glm::vec3 right_;
        glm::vec3 world_up_;
        float pitch_ = 0.0f;
        float yaw_ = -90.0f;
        float fov_ = 60.0f;
        float aspect_ = 16.0f / 9.0f;
        float near_plane_ = 0.1f;
        float far_plane_ = 1000.0f;


        glm::mat4 view_matrix;
        glm::mat4 projection_matrix;

        bool use_manual_orientation_ = false;
};


class CameraControls {
    public:
        virtual void update(Camera& camera, float delta_t) = 0;

        virtual void on_mouse_move(double xpos, double ypos);
        virtual void on_mouse_button(int button, int action, int mods);
        virtual void on_scroll(double xoffset, double yoffset);

        virtual ~CameraControls();
};


class FlyCameraControls : public CameraControls {
public:
    FlyCameraControls(GLFWwindow* window, std::shared_ptr<Camera> camera);
    void update(Camera& camera, float delta_t) override;
    void update(float delta_t);
    void on_mouse_button(int button, int action, int mods) override;
    void on_mouse_move(double xpos, double ypos) override;


private:
    GLFWwindow* window;
    std::shared_ptr<Camera> camera;

    bool rotating = false;
    bool first_mouse = true;
    double last_x = 0.0, last_y = 0.0;
    float sensitivity = 0.1f;
    float speed = 5.0f;

    float yaw = -90.0f;
    float pitch = 0.0f;

    void update_camera_vectors();
};


class OrbitalCameraControls : public CameraControls {
    public:
        OrbitalCameraControls(GLFWwindow* window, std::shared_ptr<Camera> camera, glm::vec3 target = glm::vec3(0.0f));

        void update(Camera& camera, float delta_t) override;
        void on_mouse_move(double xpos, double ypos) override;
        void on_mouse_button(int button, int action, int mods) override;
        void on_scroll(double xoffset, double yoffset) override;
        void set_angles(float new_yaw, float new_pitch);
        void set_passive_rotation(bool enabled) { passive_rotation_enabled_ = enabled; }
        void set_rotation_speed(float speed) { passive_rotation_speed_ = speed; }
        void apply_man_update();
        float get_yaw() const { return yaw; }
        float get_pitch() const { return pitch; }


    private:
        void update_camera(Camera& camera);

        GLFWwindow* window;
        std::shared_ptr<Camera> camera;
        glm::vec3 target;

        bool rotating = false;
        bool first_mouse = true;
        double last_x = 0.0, last_y = 0.0;

        float radius = 10.0f;
        float yaw = -90.0f;
        float pitch = 0.0f;

        float sensitivity = 0.3f;
        float zoom_speed = 0.5f;

        float passive_rotation_speed_ = 10.0f; // degrees per second
        bool passive_rotation_enabled_ = false;

};


class CameraInputAdapter : public input::InputListener {
public:
    CameraInputAdapter(CameraControls* controls, Camera& camera)
        : controls_(controls), camera_(camera) {}

    void update(float dt) override {
        if (controls_) controls_->update(camera_, dt);
    }

    void on_mouse_button(int button, int action, int mods) override {
        if (controls_) controls_->on_mouse_button(button, action, mods);
    }

    void on_mouse_move(double xpos, double ypos) override {
        if (controls_) controls_->on_mouse_move(xpos, ypos);
    }

    void on_scroll(double xoffset, double yoffset) override {
        if(controls_) controls_->on_scroll(xoffset, yoffset);
    }

private:
    CameraControls* controls_;
    Camera& camera_;
};
}
