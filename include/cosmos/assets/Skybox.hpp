#pragma once
// ==
// Standard Library
// ==
#include <memory>

// ==
// Third Party
// ==

// ==
// Cosmos
// ==

namespace cosmos::render { class Mesh; class Material; }

namespace cosmos::assets {

class SkyBox {

    public:
        SkyBox(std::shared_ptr<render::Mesh> mesh, std::shared_ptr<render::Material> material)
            : mesh_(mesh), material_(material) {};

        std::shared_ptr<render::Mesh> get_mesh() const { return mesh_; }
        std::shared_ptr<render::Material> get_material() const { return material_; }

        void cleanup();

    private:
        std::shared_ptr<render::Mesh> mesh_;
        std::shared_ptr<render::Material> material_;
};
}
