#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
    public:
        virtual ~Camera() = default;

        virtual glm::mat4 get_view_matrix() const = 0;
        virtual glm::mat4 get_projection_matrix(int width, int height) const = 0;
};

#endif
        
