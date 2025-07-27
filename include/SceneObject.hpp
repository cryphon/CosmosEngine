#pragma once
#include "Mesh.hpp"
#include "Material.hpp"
#include "Transform.hpp"

inline static int current_id = 0;

struct SceneObject {
    public:
        SceneObject(const std::string& name,
                std::shared_ptr<Mesh> mesh,
                std::shared_ptr<Material> mat,
                const Transform& transform)
            : name(name), mesh(mesh), material(mat), transform(transform), object_id(generate_id()) {} 

        std::string name;
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Material> material;
        Transform transform;
        int object_id;

        int get_id() const {
            return object_id;
        }

    private:
        static int generate_id() {
            return current_id++;
        }
};
