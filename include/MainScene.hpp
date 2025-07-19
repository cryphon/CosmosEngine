#include "Scene.hpp"
#include "imgui.h"
#include <memory>
#include "Renderer.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include <GLFW/glfw3.h>
#include "PerspectiveCamera.hpp"

class MainScene : public Scene {
    public:
        MainScene(Renderer* renderer, std::shared_ptr<Camera> camera)
            : renderer(renderer), camera(camera) {}

        void initialize() override { 
            // Simple textured quad
            // pos         | normal     | color       | texcoord
            static float vertices[] = {
                // pos              | normal         | color       | tex coords
                // Front face (+Z)
                -0.5f, -0.5f,  0.5f,  0, 0, 1,   1, 1, 1,   0, 0,
                0.5f, -0.5f,  0.5f,  0, 0, 1,   1, 1, 1,   1, 0,
                0.5f,  0.5f,  0.5f,  0, 0, 1,   1, 1, 1,   1, 1,
                -0.5f,  0.5f,  0.5f,  0, 0, 1,   1, 1, 1,   0, 1,

                // Back face (-Z)
                -0.5f, -0.5f, -0.5f,  0, 0, -1,  1, 1, 1,   1, 0,
                0.5f, -0.5f, -0.5f,  0, 0, -1,  1, 1, 1,   0, 0,
                0.5f,  0.5f, -0.5f,  0, 0, -1,  1, 1, 1,   0, 1,
                -0.5f,  0.5f, -0.5f,  0, 0, -1,  1, 1, 1,   1, 1,

                // Left face (-X)
                -0.5f, -0.5f, -0.5f, -1, 0, 0,   1, 1, 1,   0, 0,
                -0.5f, -0.5f,  0.5f, -1, 0, 0,   1, 1, 1,   1, 0,
                -0.5f,  0.5f,  0.5f, -1, 0, 0,   1, 1, 1,   1, 1,
                -0.5f,  0.5f, -0.5f, -1, 0, 0,   1, 1, 1,   0, 1,

                // Right face (+X)
                0.5f, -0.5f, -0.5f,  1, 0, 0,   1, 1, 1,   1, 0,
                0.5f, -0.5f,  0.5f,  1, 0, 0,   1, 1, 1,   0, 0,
                0.5f,  0.5f,  0.5f,  1, 0, 0,   1, 1, 1,   0, 1,
                0.5f,  0.5f, -0.5f,  1, 0, 0,   1, 1, 1,   1, 1,

                // Top face (+Y)
                -0.5f,  0.5f, -0.5f,  0, 1, 0,   1, 1, 1,   0, 1,
                0.5f,  0.5f, -0.5f,  0, 1, 0,   1, 1, 1,   1, 1,
                0.5f,  0.5f,  0.5f,  0, 1, 0,   1, 1, 1,   1, 0,
                -0.5f,  0.5f,  0.5f,  0, 1, 0,   1, 1, 1,   0, 0,

                // Bottom face (-Y)
                -0.5f, -0.5f, -0.5f,  0, -1, 0,  1, 1, 1,   0, 0,
                0.5f, -0.5f, -0.5f,  0, -1, 0,  1, 1, 1,   1, 0,
                0.5f, -0.5f,  0.5f,  0, -1, 0,  1, 1, 1,   1, 1,
                -0.5f, -0.5f,  0.5f,  0, -1, 0,  1, 1, 1,   0, 1,
            };

            static unsigned int indices[] = {
                0, 1, 2, 2, 3, 0,        // front
                4, 5, 6, 6, 7, 4,        // back
                8, 9,10,10,11, 8,        // left
                12,13,14,14,15,12,        // right
                16,17,18,18,19,16,        // top
                20,21,22,22,23,20         // bottom
            };
            quad_mesh = std::make_shared<Mesh>();
            quad_mesh->init(vertices, sizeof(vertices), indices, sizeof(indices));

            Light light1({1.0f, 5.0f, 2.0f}, {1.0f, 0.9f, 0.7f});
            renderer->set_light(light1);

            auto shader = std::make_shared<Shader>("shaders/default.vert", "shaders/default.frag");
            auto texture = std::make_shared<Texture>("pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
            quad_material = std::make_shared<Material>(shader, texture);
        }

        void update(float dt) override { 
            rotation += rotation_speed * dt;

            static bool prev_f1 = false;
            bool curr_f1 = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_F1) == GLFW_PRESS;

            if (curr_f1 && !prev_f1) {
                show_camera_debug = !show_camera_debug;
            }

            prev_f1 = curr_f1;
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

            if (ImGui::BeginMainMenuBar()) {
                if(ImGui::BeginMenu("Simulation Settings")) {
                ImGui::SliderFloat("Scale", &scale, 0.1f, 5.0f);
                ImGui::SliderFloat("Rotation Speed", &rotation_speed, -500.0f, 500.0f);
                ImGui::End();
                }
                if (ImGui::BeginMenu("Debug")) {
                    ImGui::MenuItem("Show Camera Panel", nullptr, &show_camera_debug);
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
                if (show_camera_debug) {
                    // Cast camera to actual type
                    if (auto persp_cam = dynamic_cast<PerspectiveCamera*>(camera.get())) {
                        glm::vec3 pos = persp_cam->get_position();
                        if (ImGui::DragFloat3("Camera Pos", &pos.x, 0.1f)) {
                            persp_cam->set_position(pos);
                        }

                        persp_cam->update_direction();
                    }
                }           
            }

        }

        void cleanup() override { }


    private:
        Renderer* renderer;
        std::shared_ptr<Camera> camera;
        std::shared_ptr<Mesh> quad_mesh;
        std::shared_ptr<Material> quad_material;

        bool show_camera_debug = false;

        float rotation = 0.0f;
        float rotation_speed = 0.1f;
        float scale = 1.0f;
};
