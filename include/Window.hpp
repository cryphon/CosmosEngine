#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>
#include <functional>
#include <chrono>


class InputManager;
class UI;
class Window;

constexpr int DEFAULT_WIDTH = 800;
constexpr int DEFAULT_HEIGHT = 600;
constexpr char const* DEFAULT_TITLE = "Cosmos Engine GL";
const glm::vec4 DEFAULT_CLEAR_COLOR = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

struct WindowData {
    Window* window = nullptr;
    InputManager* input = nullptr;
};

using ResizeCallback = std::function<void(int width, int height)>;


class Window : public std::enable_shared_from_this<Window> {
    public:
        Window(int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT,
            const char* title = DEFAULT_TITLE, bool fullscreen = false,
            int samples = 0);
    virtual ~Window();
    GLFWwindow* get_glfw_ref() const { return window_; }

    void activate();

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
        depthtest_enabled = true;
    }
    void disable_depthtest() {
        glDisable(GL_DEPTH_TEST);
        depthtest_enabled = false;
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

    void set_size(int width, int height);
    void set_resize_callback(ResizeCallback cb);
    void enable_resize_updates();
    void disable_resize_updates();

    const float* get_frame_deltas() const;
    int get_frame_deltas_offset() const;
    float get_avg_fps() const;
    glm::vec4 get_clear_color() const { return clear_color_; }
    void set_clear_color(glm::vec4 color) { clear_color_ = color; }

    void make_fullscreen();
    void make_windowed();

    void init_ui();
    std::shared_ptr<UI> get_ui() { return ui_; }

    void init_inputmanager();
    std::shared_ptr<InputManager> get_inputmanager() { return input_manager_; }

    GLFWwindow* window_;
    bool depthtest_enabled = false;
    bool stenciltest_enabled = false;

    glm::vec4 clear_color_ = DEFAULT_CLEAR_COLOR;

    bool resize_updates_enabled_ = false;
    void framebuffersize_callback(GLFWwindow* window, int width, int height);


    // Loop
    void loop(const std::function<void(float)>& frame_callback);

    private:
        std::shared_ptr<InputManager> input_manager_;
        std::shared_ptr<UI> ui_;
        std::shared_ptr<WindowData> window_data_;


        mutable std::chrono::high_resolution_clock::time_point last_fps_time_ = std::chrono::high_resolution_clock::now();
        mutable int frame_count_ = 0;
        mutable float avg_fps_ = 0.0f;
};
