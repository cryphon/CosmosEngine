#pragma once
#include "Scene.hpp"
#include <vector>
#include "SceneObject.hpp"

class RenderableScene : public Scene  {
    protected:
        std::vector<SceneObject> objects;

    public:
        virtual std::vector<SceneObject>& get_objects() { return objects; }
        virtual void add_object(const SceneObject& obj) { objects.push_back(obj); }
};
