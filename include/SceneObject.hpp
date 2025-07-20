#pragma once
#include "Mesh.hpp"
#include "Material.hpp"

struct SceneObject {
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
    glm::mat4 transform;
};
