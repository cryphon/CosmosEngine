#pragma once
#include <memory>
#include <functional>
#include <unordered_map>
#include <vector>
#include "Scene.hpp"
#include <string>

class SceneManager {
    public:
        using SceneFactory = std::function<std::unique_ptr<Scene>()>;

        // --- Register scene type (without creation)
        void register_factory(const std::string&name, SceneFactory factory) {
            factories[name] = std::move(factory);
        }

        void set_scene(const std::string& name) {
            // --- Cleanup Scene ---
            if(current) {
                current->on_exit();
                current->cleanup();
                current.reset();
            }

            // --- Create new Scene ---
            auto it = factories.find(name);
            if(it != factories.end()) {
                current = it->second();
                current_scene = name;
                current->initialize();
                current->on_enter();
            }
        }

        void reset_scene() {
            if(!current_scene.empty()) {
                set_scene(current_scene);
            }
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

        void clear() {
            if(current) {
                current->on_exit();
                current->cleanup();
                current.reset();
            }
        }

        std::vector<std::string> get_scene_names() const {
            std::vector<std::string> names;
            for (const auto& [name, _] : factories)
                names.push_back(name);
            return names;
        }

        std::string get_current_scene_name() { return current_scene; }
        Scene* get_current_scene_obj() { return current.get(); }


    private:
        std::unordered_map<std::string, SceneFactory> factories;
        std::unique_ptr<Scene> current;
        std::string current_scene;

};

