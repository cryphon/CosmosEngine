#include <glm/glm.hpp>
class Shader;

class ViewSource {
    public:
        virtual glm::mat4 get_view_transform() const = 0;
};

class UniformSource {
    public:
        virtual void update_uniforms(Shader& shader) = 0;
};
