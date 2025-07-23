#pragma once
#include "Scene.hpp"
#include <vector>
#include "SceneObject.hpp"

class RenderableScene : public Scene  {
    protected:
        std::vector<SceneObject> objects;

    public:
        virtual const std::vector<SceneObject>& get_objects() const { return objects; }
        virtual void add_object(const SceneObject& obj) { objects.push_back(obj); }
};
