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

namespace cosmos::render { class Mesh; class Material; }

namespace cosmos::assets {

class SkyBox {

    public:
        SkyBox(std::shared_ptr<render::Mesh> mesh, std::shared_ptr<render::Material> material)
            : mesh(mesh), material(material) {};

        std::shared_ptr<render::Mesh> get_mesh() const { return mesh; }
        std::shared_ptr<render::Material> get_material() const { return material; }

        void cleanup();

    private:
        std::shared_ptr<render::Mesh> mesh;
        std::shared_ptr<render::Material> material;
};
}
