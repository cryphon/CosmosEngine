#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <tuple>
#include <functional>
#include <memory>

#include <Screen.hpp>

class Camera;
class CameraControls;

constexpr int DEFAULT_WIDTH = 800;
constexpr int DEFAULT_HEIGHT = 600;
constexpr char const* DEFAULT_TITLE = "Cosmos Engine GL";
const glm::vec4 DEFAULT_CLEAR_COLOR = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

enum class EscBehaviour {
    NONE,
    TOGGLE_MOUSE_CAPTURE,
    CLOSE,
    UNCAPTURE_MOUSE_OR_CLOSE,
};

enum class MouseButtonBehaviour {
    NONE,
    CAPTURE_MOUSE,
};


class Window {
    public:
        Window(int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT,
            const char* title = DEFAULT_TITLE, bool fullscreen = false,
            int samples = 0);
    virtual ~Window();
    GLFWwindow* get_glfw_ref() const { return window_; }

    void activate();
    void set_viewport() {
        ImageSize size = get_size();
        glViewport(0, 0, size.width, size.height);
    }

    void enable_vsync() {
        activate();
        glfwSwapInterval(1);
    }
    void disable_vsync() {
        activate();
        glfwSwapInterval(0);
    }

    void enable_depthtest() {
        glEnable(GL_DEPTH_TEST);
        depthtest_enabled_ = true;
    }
    void disable_depthtest() {
        glDisable(GL_DEPTH_TEST);
        depthtest_enabled_ = false;
    }

    void enableAlphaBlending() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);
    }
    void disable_alphablending() { glDisable(GL_BLEND); }

    void enable_facecull() { glEnable(GL_CULL_FACE); }
    void disable_facecull() { glDisable(GL_CULL_FACE); }

    void enable_wireframe() { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
    void disable_wireframe() { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }

    void enable_seamless_cubemap() { glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); }
    void disable_seamless_cubemap() { glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS); }

    void cull_front_faces() { glCullFace(GL_FRONT); }
    void cull_back_faces() { glCullFace(GL_BACK); }

    ImageSize get_size() const;
    void set_size(int width, int height);
    void enable_resize_updates();
    void disable_resize_updates();
    float get_aspect_ratio() const;

    unsigned int get_frame_count() const { return frame_count_; }
    const float* get_frame_deltas() const;
    int get_frame_deltas_offset() const;
    float get_avg_fps() const;
    glm::vec4 get_clear_color() const { return clear_color_; }
    void set_clear_color(glm::vec4 color) { clear_color_ = color; }

    void make_fullscreen();
    void make_windowed();

    MouseButtonBehaviour get_mousebutton_behaviour() const {
        return mousebutton_behaviour_;
    }
    void set_mousebutton_behaviour(MouseButtonBehaviour behaviour) {
        mousebutton_behaviour_ = behaviour;
    }

    void enable_keyinput();
    void disable_keyinput();
    void enable_scrollinput();
    void disable_scrollinput();
    void enable_mousemoveinput();
    void disable_mousemoveinput();
    void enable_mousebuttoninput();
    void disable_mousebuttoninput();

    void enable_mousecapture();
    void disable_mousecapture();

    void bind_camera(std::shared_ptr<Camera> camera);
    void bind_cameracontrols(std::shared_ptr<CameraControls> camera_controls);

    // TODO: Add a way to remove handlers.
    void add_keypress_handler(int glfw_key, std::function<void(int)> handler);
    void add_mousebutton_handler(int glfw_mousebutton,
            std::function<void(int)> handler);

    void loop(std::function<void(float)> callback);


    private:
    void process_input(float delta_time);
    void key_callback(int key, int scancode, int action, int mods);
    void scroll_callback(double xoffset, double yoffset);
    void mousemove_callback(double xpos, double ypos);
    void mousebutton_callback(int button, int action, int mods);
    void framebuffersize_callback(GLFWwindow* window, int width, int height);

    void update_framestats(float delta_time);

    GLFWwindow* window_;
    bool depthtest_enabled_ = false;
    bool stenciltest_enabled_ = false;

    float last_time_ = 0.0f;
    float delta_time_ = 0.0f;
    unsigned int frame_count_ = 0;
    static constexpr int NUM_FRAME_DELTAS = 120;
    float frame_deltas_[NUM_FRAME_DELTAS] = {0.0f};
    float frame_delta_sum_ = 0.0f;
    glm::vec4 clear_color_ = DEFAULT_CLEAR_COLOR;

    EscBehaviour esc_behaviour_ = EscBehaviour::NONE;
    MouseButtonBehaviour mousebutton_behaviour_ = MouseButtonBehaviour::NONE;
    bool resize_updates_enabled_ = false;
    bool keyinput_enabled_ = false;
    bool scrollinput_enabled_ = false;
    bool mousemove_input_enabled_ = false;
    bool mousebutton_input_enabled_ = false;
    bool keyinput_paused_ = false;
    bool mouseinput_paused_ = false;
    bool mouse_captured_ = false;

    std::vector<std::tuple<int, std::function<void(int)>>> keypress_handlers_;
    std::vector<std::tuple<int, std::function<void(int)>>> mousebutton_handlers_;

    std::shared_ptr<Camera> bound_camera_ = nullptr;
    std::shared_ptr<CameraControls> bound_cameracontrols_ = nullptr;
};
