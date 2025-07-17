#include "Scene.hpp"
#include "imgui.h"
#include <memory>
#include "Renderer.hpp"
#include "Camera.hpp"

class MainScene : public Scene {
    public:
        MainScene(Renderer* renderer, std::shared_ptr<Camera> camera)
            : renderer(renderer), camera(camera) {}

        void initialize() override { }
        void update(float dt) override { 
            rotation += rotation_speed * dt;
        }
        void render() override { 
            if(camera && renderer) {
                renderer->render(scale, rotation, *camera, 1000, 1000);
            }
        }
        void render_ui() override {
            ImGui::Begin("Simulation Settings");
            ImGui::SliderFloat("Scale", &scale, 0.1f, 5.0f);
            ImGui::SliderFloat("Rotation Speed", &rotation_speed, -5.0f, 5.0f);
            ImGui::End();
        }

        void cleanup() override { }


    private:
        Renderer* renderer;
        std::shared_ptr<Camera> camera;
        float rotation = 0.0f;
        float rotation_speed = 0.1f;
        float scale = 1.0f;
};
