#pragma once
// ==
// Standard Library
// ==

// ==
// Third Party
// ==
#include <glm/glm.hpp>

// ==
// Cosmos
// ==

namespace cosmos::render {
struct UniformContext {
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 view_pos;
    glm::vec3 light_pos;
    glm::vec3 light_color;
    glm::mat4 model;
    float reflectivity;
    float alpha;
};
}
