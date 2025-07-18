#include "Shader.hpp"
#include "Texture.hpp"
#include <memory>

class Material {
    public:
        Material(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture);
        void bind();
        std::shared_ptr<Shader> shader;
        std::shared_ptr<Texture> texture;
};
