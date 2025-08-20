#pragma once
// ==
// Standard Library
// ==
#include <memory>
#include <functional>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <iostream>

// ==
// Third Party
// ==
#include <glad/glad.h>

// ==
// Cosmos
// ==
#include <cosmos/render/Shader.hpp>
#include <cosmos/assets/Skybox.hpp>

namespace cosmos::assets {
class SkyBoxManager {
    public:
        using SkyBoxFactory = std::function<std::shared_ptr<SkyBox>()>;

        void register_factory(const std::string&name, SkyBoxFactory factory) {
            factories_[name] = std::move(factory);
        }

        void set_skybox(const std::string& name) {
            // cleanup??

            auto it = factories_.find(name);
            if(it != factories_.end()) {
                current_skybox_name_ = name;
                current_skybox_ = it->second();
            }
        }

        std::shared_ptr<SkyBox> get_current_skybox() { return current_skybox_; }

        std::vector<std::string> get_skybox_names() const {
            std::vector<std::string> names;
            for (const auto& [name, _] : factories_)
                names.push_back(name);
            return names;
        }

        std::string get_current_skybox_name() { return current_skybox_name_; }

    private:
        std::unordered_map<std::string, SkyBoxFactory> factories_;
        std::shared_ptr<SkyBox> current_skybox_;
        std::string current_skybox_name_;

};
}
