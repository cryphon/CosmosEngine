#pragma once
#include "Scene.hpp"
#include <vector>
#include <memory>
#include "SceneObject.hpp"
#include "Skybox.hpp"

class RenderableScene : public Scene  {
    protected:
        std::vector<SceneObject> objects;
        std::shared_ptr<SkyBox> skybox;

    public:
        virtual std::vector<SceneObject>& get_objects() { return objects; }
        virtual void add_object(const SceneObject& obj) { objects.push_back(obj); }
        void set_skybox(std::shared_ptr<SkyBox> new_skybox) { skybox = new_skybox; }
        
};
