#include <glm/glm.hpp>
#include <memory>
#include "Transform.hpp"
class Mesh;
class Material;

class RenderCommand {
    public:
        void bind();
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Material> material;
        Transform transform;
};
