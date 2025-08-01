#include "CameraControls.hpp"
constexpr float POLAR_CAP = 90.0f - 0.1f;

void CameraControls::handle_dragstartend(int button, int action) {
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        dragging_ = true;
        initialized_ = false;
    } else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        dragging_ = false;
    }
}

MouseDelta CameraControls::calculate_mousedelta(double xpos, double ypos) {
    if(!initialized_) {
        last_mouse_x_ = xpos;
        last_mouse_y_ = ypos;
        initialized_= true;
    }
    float x_offset = xpos - last_mouse_x_;
    float y_offset = last_mouse_y_ - ypos;
    last_mouse_x_ = xpos;
    last_mouse_y_ = ypos;
    return MouseDelta{x_offset, y_offset};
}

/*
   ###############################################################################
   ##                           FlyCameraControls                               ##
   ###############################################################################
*/


void FlyCameraControls::resize_window(int width, int height) {}
void FlyCameraControls::scroll(Camera& camera, double xoffset, double yoffset, bool mouse_captured) {
    camera.zoom(yoffset * sensitivity_);
}

void FlyCameraControls::mouse_move(Camera& camera, double xpos, double ypos, bool mouse_captured) {
    if(!(dragging_ || mouse_captured)) {
        return;
    }
    
    MouseDelta delta = calculate_mousedelta(xpos, ypos);
    camera.rotate(delta.xoffset * sensitivity_, delta.yoffset * sensitivity_);
}

void FlyCameraControls::mouse_button(Camera& camera, int button, int action, int mods, bool mouse_captured) {
    handle_dragstartend(button, action);
}

void FlyCameraControls::process_input(GLFWwindow* window, Camera& camera,
                                     float delta_time) {
  float vel = speed_ * delta_time;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.move(CameraDirection::FORWARD, vel);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.move(CameraDirection::LEFT, vel);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.move(CameraDirection::BACKWARD, vel);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.move(CameraDirection::RIGHT, vel);
  }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    camera.move(CameraDirection::UP, vel);
  }
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    camera.move(CameraDirection::DOWN, vel);
  }
}

/*
   ###############################################################################
   ##                           OrbitCameraControls                             ##
   ###############################################################################
*/

OrbitCameraControls::OrbitCameraControls(Camera& camera, glm::vec3 center)
    : center_(center) {
  glm::vec3 dir = camera.get_pos() - center;
  radius_ = glm::clamp(glm::length(dir), MIN_RADIUS, MAX_RADIUS);
  glm::vec3 normdir = glm::normalize(dir);
  // TODO: Fix this, atan2 args are backwards.
  azimuth_ = glm::mod<float>(glm::degrees(atan2(normdir.x, normdir.z)) * -1.0f,
                             360.0f) +
             90.0f;
  altitude_ =
      glm::clamp<float>(glm::degrees(asin(normdir.y)), -POLAR_CAP, POLAR_CAP);

  update_camera(camera);
}

void OrbitCameraControls::resize_window(int width, int height) {}

void OrbitCameraControls::scroll(Camera& camera, double xoffset, double yoffset,
                                 bool mouse_captured) {
  radius_ = glm::clamp(radius_ - static_cast<float>(yoffset * sensitivity_),
                       MIN_RADIUS, MAX_RADIUS);
  update_camera(camera);
}

void OrbitCameraControls::mouse_move(Camera& camera, double xpos, double ypos,
                                    bool mouse_captured) {
  if (!(dragging_ || mouse_captured)) {
    return;
  }

  MouseDelta delta = calculate_mousedelta(xpos, ypos);

  // Constrain azimuth to be 0-360 to avoid floating point error.
  azimuth_ = glm::mod(azimuth_ + delta.xoffset * sensitivity_, 360.0f);
  altitude_ = glm::clamp(altitude_ - delta.yoffset * sensitivity_, -POLAR_CAP,
                         POLAR_CAP);

  update_camera(camera);
}

void OrbitCameraControls::mouse_button(Camera& camera, int button, int action,
                                      int mods, bool mouse_captured) {
  handle_dragstartend(button, action);
}

void OrbitCameraControls::process_input(GLFWwindow* window, Camera& camera,
                                       float delta_time) {}

void OrbitCameraControls::update_camera(Camera& camera) {
  // Compute camera position.
  glm::vec3 camera_pos;
  camera_pos.x = center_.x + radius_ * cos(glm::radians(altitude_)) *
                                     cos(glm::radians(azimuth_));
  camera_pos.y = center_.y + radius_ * sin(glm::radians(altitude_));
  camera_pos.z = center_.z + radius_ * cos(glm::radians(altitude_)) *
                                     sin(glm::radians(azimuth_));

  camera.set_pos(camera_pos);
  camera.look_at(center_);
}

