#pragma once

#include <memory>
#include <functional>
#include <unordered_map>
#include <vector>
#include "Skybox.hpp"
#include <string>
#include <memory>
#include <glad/glad.h>
#include <Shader.hpp>
#include <iostream>

class SkyBoxManager {
    public:
        using SkyBoxFactory = std::function<std::shared_ptr<SkyBox>()>;

        void register_factory(const std::string&name, SkyBoxFactory factory) {
            factories[name] = std::move(factory);
        }

        void set_skybox(const std::string& name) {
            // cleanup??

            auto it = factories.find(name);
            if(it != factories.end()) {
                current_skybox_name = name;
                current_skybox = it->second();
            }
        }

        std::shared_ptr<SkyBox> get_current_skybox() { return current_skybox; }

        std::vector<std::string> get_skybox_names() const {
            std::vector<std::string> names;
            for (const auto& [name, _] : factories)
                names.push_back(name);
            return names;
        }

        std::string get_current_skybox_name() { return current_skybox_name; }

    private:
        std::unordered_map<std::string, SkyBoxFactory> factories;
        std::shared_ptr<SkyBox> current_skybox;
        std::string current_skybox_name;

};

