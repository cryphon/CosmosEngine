#pragma once
#include "Mesh.hpp"
#include "Material.hpp"
#include "Transform.hpp"

struct SceneObject {
    SceneObject(char* name,
            std::shared_ptr<Mesh> mesh,
            std::shared_ptr<Material> mat,
            const Transform& transform)
        : name(name), mesh(mesh), material(mat), transform(transform) {} 

    char* name;
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
    Transform transform;
};
