#include <cosmos/scene/RenderableScene.hpp>
#include <memory>

class Camera;
class SkyBox;

namespace cosmos::render { class Renderer; class Mesh; class Material; }
namespace cosmos::scene { class Camera; }
namespace cosmos::assets { class SkyBox; }


class MainScene : public cosmos::scene::RenderableScene {
public:
    MainScene(cosmos::render::Renderer* renderer, std::shared_ptr<cosmos::scene::Camera> camera)
    : renderer(renderer), camera(camera) {}

    void initialize() override;
    void cleanup() override;
    void on_enter() override;
    void on_exit() override;
    void update(float dt) override;
    void render() override;
    void render_ui() override;


private:
    cosmos::render::Renderer* renderer;
    std::shared_ptr<cosmos::scene::Camera> camera;

    float rotation = 0.0f;
    float rotation_speed = 0.1f;
    float scale = 1.0f;   
};
