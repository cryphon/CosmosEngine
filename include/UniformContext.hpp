#pragma once
#include <glm/glm.hpp>

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
