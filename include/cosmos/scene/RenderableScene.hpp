#pragma once
// ==
// Standard Library
// ==
#include <vector>
#include <memory>

// ==
// Third Party
// ==

// ==
// Cosmos
// ==
#include <cosmos/scene/Scene.hpp>

// ==
// Forward Declare
// ==
namespace cosmos::assets { class SkyBox; }
namespace cosmos::scene { class SceneObject; }

namespace cosmos::scene {

class RenderableScene : public Scene  {
    protected:
        std::vector<SceneObject> objects;
        std::shared_ptr<assets::SkyBox> skybox;

    public:
        virtual std::vector<SceneObject>& get_objects() { return objects; }
        virtual void add_object(const SceneObject& obj) { objects.push_back(obj); }
        void set_skybox(std::shared_ptr<assets::SkyBox> new_skybox) { skybox = new_skybox; }
        
};
}
