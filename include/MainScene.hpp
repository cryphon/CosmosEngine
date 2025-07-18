#include "Scene.hpp"
#include "imgui.h"
#include <memory>
#include "Renderer.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"

class MainScene : public Scene {
    public:
        MainScene(Renderer* renderer, std::shared_ptr<Camera> camera)
            : renderer(renderer), camera(camera) {}

        void initialize() override { 
            // Simple textured quad
            static float vertices[] = {
                //  pos        | color      | tex coords
                -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
                0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
                0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
                -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f
            };

            static unsigned int indices[] = {
                0, 1, 2,
                2, 3, 0
            };

            quad_mesh = std::make_shared<Mesh>();
            quad_mesh->init(vertices, sizeof(vertices), indices, sizeof(indices));

            auto shader = std::make_shared<Shader>("shaders/default.vert", "shaders/default.frag");
            auto texture = std::make_shared<Texture>("pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
            quad_material = std::make_shared<Material>(shader, texture);
        }

        void update(float dt) override { 
            rotation += rotation_speed * dt;
        }
        void render() override { 
            if (camera && renderer && quad_mesh && quad_material) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(scale));
            model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));

            renderer->submit({ quad_mesh, quad_material, model });
        }

        renderer->render_all(*camera, 1000, 1000);
        renderer->clear();        }
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
        std::shared_ptr<Mesh> quad_mesh;
        std::shared_ptr<Material> quad_material;


        float rotation = 0.0f;
        float rotation_speed = 0.1f;
        float scale = 1.0f;
};
