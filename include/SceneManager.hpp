#pragma once
#include <memory>
#include "Scene.hpp"

class SceneManager {
    public:
        void set_scene(std::unique_ptr<Scene> new_scene) {
            if(current) current->cleanup();
            current = std::move(new_scene);
            current->initialize();
        }

        void update(float dt) {
            if(current) current->update(dt);
        }

        void render() {
            if(current) current->render();
        }

        void render_ui() {
            if(current) current->render_ui();
        }

    private:
            std::unique_ptr<Scene> current;

};

