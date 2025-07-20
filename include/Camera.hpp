#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
    public:
        virtual ~Camera() = default;

        virtual void update_view() = 0;
        virtual void update_projection() = 0;
        virtual glm::vec3 get_position() const  =  0;
        glm::mat4 get_view_matrix() const { return view; }
        glm::mat4 get_projection_matrix() const { return projection; }

        void set_aspect_ratio(float ratio) {
            aspect_ratio = ratio;
            update_projection();
        }
    protected:
        float aspect_ratio = 1.0f;
        glm::mat4 projection = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
};
#endif
        
