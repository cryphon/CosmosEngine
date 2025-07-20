#include "Mesh.hpp"
#include "Material.hpp"
#include <glm/glm.hpp>

class RenderCommand {
    public:
        void bind();
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Material> material;
        glm::mat4 transform;
        int object_id = -1;
};
