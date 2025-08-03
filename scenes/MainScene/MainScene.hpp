#include "RenderableScene.hpp"
#include "imgui.h"
#include <memory>

class Renderer;
class Camera;
class Mesh;
class Material;

class MainScene : public RenderableScene {
    public:
        MainScene(Renderer* renderer, std::shared_ptr<Camera> camera)
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

        float rotation = 0.0f;
        float rotation_speed = 0.1f;
        float scale = 1.0f;   
};
