#pragma once

#include <string>
#include <memory>

class Mesh;
class Material;

class SkyBox {

    public:
        SkyBox(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
            : mesh(mesh), material(material) {};

        std::shared_ptr<Mesh> get_mesh() const { return mesh; }
        std::shared_ptr<Material> get_material() const { return material; }

        void cleanup();

    private:
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Material> material;
};