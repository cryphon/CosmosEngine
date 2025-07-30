#pragma once
#include "Scene.hpp"
#include <vector>
#include "SceneObject.hpp"
#include <memory>
#include <unordered_map>
#include "Logger.hpp"

class Camera;

class RenderableScene : public Scene  {
    protected:
        std::vector<SceneObject> objects;
        std::shared_ptr<Camera> active_camera;
        std::unordered_map<std::string, std::shared_ptr<Camera>> cameras;

    public:
        virtual std::vector<SceneObject>& get_objects() { return objects; }
        virtual void add_object(const SceneObject& obj) { objects.push_back(obj); }
        void add_camera(const std::string& name, std::shared_ptr<Camera> cam) {
            cameras[name] = cam;
            if(!active_camera) {
                LOG_INFO("First camera instance?");
                active_camera = cam;
            }
        }

        std::vector<std::string> get_camera_names() const {
            std::vector<std::string> names;
            for (const auto& pair : cameras) {
                names.push_back(pair.first);
            }
            return names;
        }
        std::string get_active_camera_name() const {
            for (const auto& [name, cam] : cameras) {
                if (cam == active_camera) return name;
            }
            return "unknown";
        }

        void set_active_camera(const std::string& name) {
            auto it = cameras.find(name);
            if (it != cameras.end()) {
                active_camera = it->second;
            } else {
                LOG_INFO("Camera '" + name + "' not found in scene.");
            }
        }

        std::shared_ptr<Camera> get_active_camera() const { 
            if (!active_camera) {
                LOG_ERROR("RenderableScene: no active camera set!");
            }
            return active_camera;
        }
};
