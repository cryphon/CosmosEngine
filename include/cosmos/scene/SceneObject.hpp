#pragma once

// ==
// Standard Library
// ==
#include <string>
#include <memory>

// ==
// Third Party
// ==

// ==
// Cosmos
// ==

// ==
// Forward Declare
// ==
#include <cosmos/render/Transform.hpp>

namespace cosmos::render { class Mesh; class Material; }


namespace cosmos::scene {

inline static int current_id = 0;

struct SceneObject {
public:
    SceneObject(const std::string& name,
                std::shared_ptr<render::Mesh> mesh,
                std::shared_ptr<render::Material> mat,
                const render::Transform& transform)
    : name(name), mesh(mesh), material(mat), transform(transform), object_id(generate_id()) {} 

    std::string name;
    std::shared_ptr<render::Mesh> mesh;
    std::shared_ptr<render::Material> material;
    render::Transform transform;
    int object_id;

    int get_id() const {
        return object_id;
    }

private:
    static int generate_id() {
        return current_id++;
    }
};
}
