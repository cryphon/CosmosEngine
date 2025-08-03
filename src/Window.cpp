#include "Window.hpp"
#include "Logger.hpp"
#include "Camera.hpp"
#include "Ui.hpp"
#include "InputManager.hpp"


bool glfwErrorLoggingEnabled = true;
ResizeCallback resize_callback_;

void glfwErrorCallback(int error, const char* description) {
  if (glfwErrorLoggingEnabled) {
    fprintf(stderr, "GLFW ERROR: %s [error code %d]\n", description, error);
  }
}


Window::Window(int width, int height, const char* title, bool fullscreen, int samples) {
    glfwSetErrorCallback(glfwErrorCallback);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    if(samples > 0) {
        glfwWindowHint(GLFW_SAMPLES, samples);
    }

    // nullptr = windowed
    GLFWmonitor* monitor = nullptr;
    if(fullscreen) {
        monitor = glfwGetPrimaryMonitor();
    }

    window_ = glfwCreateWindow(width, height, title, monitor, nullptr);

    if(window_ == nullptr) {
        LOG_ERROR("Window creation failed");
    }
    
    activate();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("Failed to initialize GLAD");
    }


    window_data_ = std::make_shared<WindowData>();
    window_data_->window = this;
    glfwSetWindowUserPointer(window_, window_data_.get());


    // enable multi-sampling
    if(samples > 0) {
        glEnable(GL_MULTISAMPLE);
    }

    enable_depthtest();
    enable_seamless_cubemap();
    enable_resize_updates();
};


Window::~Window() {
    if(window_ != nullptr) {
        glfwDestroyWindow(window_);
    }
}

void Window::activate() { glfwMakeContextCurrent(window_); }

void Window::set_size(int width, int height) {
    glfwSetWindowSize(window_, width, height);
}

void Window::enable_resize_updates() {
    if(resize_updates_enabled_) return;

    auto callback = [](GLFWwindow* window, int width, int height) {
        auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (self) {
            self->framebuffersize_callback(window, width, height);
        }
    };

    glfwSetFramebufferSizeCallback(window_, callback);

    int w, h;
    glfwGetFramebufferSize(window_, &w, &h);
    framebuffersize_callback(window_, w, h);

    resize_updates_enabled_ = true;
}



void Window::framebuffersize_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    if (resize_callback_) {
        resize_callback_(width, height);
    }}

void Window::init_ui() {
    ui_ = std::make_shared<UI>();
}

void Window::init_inputmanager() {
    input_manager_ = std::make_shared<InputManager>(get_glfw_ref());
    window_data_->input = input_manager_.get();
}


void Window::set_resize_callback(ResizeCallback cb) {
    resize_callback_ = std::move(cb);
}
