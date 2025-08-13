#pragma once
// ==
// Standard Library
// ==

// ==
// Third Party
// ==
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

// ==
// Cosmos
// ==


namespace cosmos::render {
class Transform {
    public:
        glm::vec3 position{0.0f};
        glm::vec3 rotation{0.0f};
        glm::vec3 scale{1.0f};

        glm::mat4 model_matrix;
        glm::mat4 inverse_matrix;
        glm::mat3 normal_matrix;

        bool cache_trigger = true;

        void update_matrices() {
            if (!cache_trigger) return;

            glm::mat4 T = glm::translate(glm::mat4(1.0f), position);
            glm::mat4 R = glm::toMat4(glm::quat(glm::radians(rotation)));
            glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);

            model_matrix = T * R * S;

            inverse_matrix = glm::inverse(model_matrix);
            normal_matrix = glm::transpose(glm::inverse(glm::mat3(model_matrix)));

            cache_trigger = false;
        }
};
}


