#include "Window.hpp"
#include "Logger.hpp"
#include "Camera.hpp"
#include "CameraControls.hpp"


bool glfwErrorLoggingEnabled = true;

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

    glfwSetWindowUserPointer(window_, this);

    // enable multi-sampling
    if(samples > 0) {
        glEnable(GL_MULTISAMPLE);
    }

    enable_depthtest();
    enable_seamless_cubemap();
    enable_resize_updates();
    enable_keyinput();
    enable_scrollinput();
    enable_mousemoveinput();
    enable_mousebuttoninput();
};


Window::~Window() {
    if(window_ != nullptr) {
        glfwDestroyWindow(window_);
    }
}

void Window::activate() { glfwMakeContextCurrent(window_); }

ImageSize Window::get_size() const {
    ImageSize size;
    glfwGetWindowSize(window_, &size.width, &size.height);
    return size;
}

void Window::set_size(int width, int height) {
    glfwSetWindowSize(window_, width, height);
}

void Window::enable_resize_updates() {
    if(resize_updates_enabled_) return;
    auto callback = [](GLFWwindow* window, int width, int height) {
        auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->framebuffersize_callback(window, width, height);
    };
    glfwSetFramebufferSizeCallback(window_, callback);
    resize_updates_enabled_ = true;
}

void Window::disable_resize_updates() {
    if(!resize_updates_enabled_) return;
    glfwSetFramebufferSizeCallback(window_, nullptr);
    resize_updates_enabled_ = false;
}

float Window::get_aspect_ratio() const {
    ImageSize size = get_size();
    return size.width / static_cast<float>(size.height);
}

void Window::enable_keyinput() {
    if(keyinput_enabled_) return;
    auto callback = [](GLFWwindow* window, int key, int scancode, int action,
                     int mods) {
    auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->key_callback(key, scancode, action, mods);
  };
  glfwSetKeyCallback(window_, callback);
  keyinput_enabled_ = true;
}

void Window::disable_keyinput() {
  if (!keyinput_enabled_) return;
  glfwSetKeyCallback(window_, nullptr);
  keyinput_enabled_ = false;
}

void Window::enable_scrollinput() {
  if (scrollinput_enabled_) return;
  auto callback = [](GLFWwindow* window, double xoffset, double yoffset) {
    auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->scroll_callback(xoffset, yoffset);
  };
  glfwSetScrollCallback(window_, callback);
  scrollinput_enabled_ = true;
}

void Window::disable_scrollinput() {
  if (!scrollinput_enabled_) return;
  glfwSetScrollCallback(window_, nullptr);
  scrollinput_enabled_ = false;
}


void Window::enable_mousemoveinput() {
  if (mousemove_input_enabled_) return;
  auto callback = [](GLFWwindow* window, double xpos, double ypos) {
    auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->mousemove_callback(xpos, ypos);
  };
  glfwSetCursorPosCallback(window_, callback);
  mousemove_input_enabled_ = true;
}

void Window::disable_mousemoveinput() {
  if (!mousemove_input_enabled_) return;
  glfwSetCursorPosCallback(window_, nullptr);
  mousemove_input_enabled_ = false;
}

void Window::enable_mousebuttoninput() {
  if (mousebutton_input_enabled_) return;
  auto callback = [](GLFWwindow* window, int button, int action, int mods) {
    auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->mousebutton_callback(button, action, mods);
  };
  glfwSetMouseButtonCallback(window_, callback);
  mousebutton_input_enabled_ = true;
}

void Window::disable_mousebuttoninput() {
  if (!mousebutton_input_enabled_) return;
  glfwSetMouseButtonCallback(window_, nullptr);
  mousebutton_input_enabled_ = false;
}

void Window::framebuffersize_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    if(bound_camera_) {
        bound_camera_->set_aspect_ratio(width / static_cast<float>(height));
    }
    if(bound_cameracontrols_) {
        bound_cameracontrols_->resize_window(width, height);
    }
}

void Window::make_fullscreen() {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    auto size = get_size();
    glfwSetWindowMonitor(window_, monitor, 0, 0, size.width, size.height, GLFW_DONT_CARE);
}
void Window::make_windowed() {
    auto size = get_size();
    glfwSetWindowMonitor(window_, nullptr, 0, 0, size.width, size.height, GLFW_DONT_CARE);
}

void Window::process_input(float delta_time) {
    if(bound_cameracontrols_) {
        bound_cameracontrols_->process_input(window_, *bound_camera_, delta_time);
    }
}

void Window::key_callback(int key, int scancode, int action, int mods) {
  if (keyinput_paused_) return;

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    if (esc_behaviour_ == EscBehaviour::TOGGLE_MOUSE_CAPTURE) {
      auto input_mode = glfwGetInputMode(window_, GLFW_CURSOR);
      if (input_mode == GLFW_CURSOR_NORMAL) {
        enable_mousecapture();
      } else {
        disable_mousecapture();
      }
    } else if (esc_behaviour_ == EscBehaviour::CLOSE) {
      glfwSetWindowShouldClose(window_, true);
    } else if (esc_behaviour_ == EscBehaviour::UNCAPTURE_MOUSE_OR_CLOSE) {
      auto inputMode = glfwGetInputMode(window_, GLFW_CURSOR);
      if (inputMode == GLFW_CURSOR_DISABLED) {
        disable_mousecapture();
      } else {
        // Close since mouse is not captured.
        glfwSetWindowShouldClose(window_, true);
      }
    }
  }

  // Run handlers.
  if (action == GLFW_PRESS) {
    for (auto pair : keypress_handlers_) {
      int glfwKey;
      std::function<void(int)> handler;
      std::tie(glfwKey, handler) = pair;

      if (key == glfwKey) {
        handler(mods);
      }
    }
  }
}


void Window::scroll_callback(double xoffset, double yoffset) {
    if(mouseinput_paused_) return;

    if(bound_cameracontrols_) {
        bound_cameracontrols_->scroll(*bound_camera_, xoffset, yoffset,
                mouse_captured_);
    }
}


void Window::mousemove_callback(double xpos, double ypos) {
    if(mouseinput_paused_) return;

    if(bound_cameracontrols_) {
        bound_cameracontrols_->mouse_move(*bound_camera_, xpos, ypos, mouse_captured_);
    }
}

void Window::mousebutton_callback(int button, int action, int mods) {
    if(mouseinput_paused_) return;

    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if(mousebutton_behaviour_ == MouseButtonBehaviour::CAPTURE_MOUSE) {
            enable_mousecapture();
        }
    }
    if (action == GLFW_PRESS) {
        for (auto pair : mousebutton_handlers_) {
            int glfw_mousebutton;
            std::function<void(int)> handler;
            std::tie(glfw_mousebutton, handler) = pair;

            if (button == glfw_mousebutton) {
                handler(mods);
            }
        }
    }
    if (bound_cameracontrols_) {
        bound_cameracontrols_->mouse_button(*bound_camera_, button, action, mods,
                mouse_captured_);
    }
}

void Window::add_keypress_handler(int glfw_key, std::function<void(int)> handler) {
    keypress_handlers_.push_back(std::make_tuple(glfw_key, handler));
}

void Window::add_mousebutton_handler(int glfw_mousebutton, std::function<void(int)> handler) {
  mousebutton_handlers_.push_back(std::make_tuple(glfw_mousebutton, handler));
}

void Window::enable_mousecapture() {
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    mouse_captured_ = true;
}

void Window::disable_mousecapture() {
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    mouse_captured_ = false;
}


void Window::bind_camera(std::shared_ptr<Camera> camera) {
    bound_camera_ = camera;
    bound_camera_->set_aspect_ratio(get_size());
}

void Window::bind_cameracontrols(std::shared_ptr<CameraControls> cameracontrols) {
    if(!bound_camera_) LOG_ERROR("Camera must be bound before camera controls");
    bound_cameracontrols_ = cameracontrols;
    ImageSize size = get_size();
    bound_cameracontrols_->resize_window(size.width, size.height);
}

const float* Window::get_frame_deltas() const { return &frame_deltas_[0]; }
int Window::get_frame_deltas_offset() const {
    return frame_count_ % NUM_FRAME_DELTAS;
}


float Window::get_avg_fps() const {
    int denominator = std::min<int>(frame_count_, NUM_FRAME_DELTAS);
    float avg_frame_delta = frame_delta_sum_ / denominator;
    return 1.0f / avg_frame_delta;
}

void Window::update_framestats(float delta_time) {
  unsigned int offset = get_frame_deltas_offset();
  float old_delta_time = frame_deltas_[offset];
  frame_delta_sum_ -= old_delta_time;
  frame_delta_sum_ += delta_time;
  frame_deltas_[offset] = delta_time;
}


void Window::loop(std::function<void(float)> callback) {
  while (!glfwWindowShouldClose(window_)) {
    float currentTime;
    delta_time_ = currentTime - last_time_;
    last_time_ = currentTime;

    update_framestats(delta_time_);

    // Clear the appropriate buffers.
    glClearColor(clear_color_.r, clear_color_.g, clear_color_.b, clear_color_.a);
    auto clearBits = GL_COLOR_BUFFER_BIT;
    if (depthtest_enabled_) {
      clearBits |= GL_DEPTH_BUFFER_BIT;
    }
    if (stenciltest_enabled_) {
      clearBits |= GL_STENCIL_BUFFER_BIT;
    }
    glClear(clearBits);

    // Process necessary input.
    process_input(delta_time_);

    // Call the loop function.
    callback(delta_time_);


    glfwSwapBuffers(window_);
    glfwPollEvents();

    ++frame_count_;
  }
}

