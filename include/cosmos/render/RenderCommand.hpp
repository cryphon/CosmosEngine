#pragma once
// ==
// Standard Library
// ==
#include <memory>

// ==
// Third Party
// ==
#include <glm/glm.hpp>

// ==
// Cosmos
// ==
#include <cosmos/render/Transform.hpp>

// ==
// Forward Declare
// ==
namespace cosmos::render { class Mesh; class Material; }

namespace cosmos::render {
class RenderCommand {
public:
    void bind();
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
    Transform transform;
    int object_id;
};

}
