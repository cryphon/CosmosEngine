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

namespace cosmos::scene {

enum class LightType {
    Point
    // add more types later
};

class Light {
    public:
        Light(glm::vec3 pos, glm::vec3 color, LightType t = LightType::Point)
            : position(pos), color(color), type(t) {}
        glm::vec3 position;
        glm::vec3 color;
        LightType type;
};
}

