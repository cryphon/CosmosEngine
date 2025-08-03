#include "imgui.h"
#include <memory>
#include "RenderableScene.hpp"

class Renderer;
class Camera;
class Mesh;
class Material;

class SecondScene : public RenderableScene {
    public:
        SecondScene(Renderer* renderer, std::shared_ptr<Camera> camera)
            : renderer(renderer), camera(camera) {}

        void initialize() override;
        void cleanup() override;
        void on_enter() override;
        void on_exit() override;
        void update(float dt) override;
        void render() override;
        void render_ui() override;


    private:
        Renderer* renderer;
        std::shared_ptr<Camera> camera;
        std::shared_ptr<Mesh> quad_mesh;
        std::shared_ptr<Material> quad_material;

        float rotation = 0.0f;
        float rotation_speed = 0.1f;
        float scale = 1.0f;   
};
