#pragma once
// ==
// Standard Library
// ==
#include <unordered_map>
#include <memory>
#include <filesystem>

// ==
// Third Party
// ==

// ==
// Cosmos
// ==
#include <cosmos/render/PBRMaterial.hpp>
#include <cosmos/render/Texture.hpp>
#include <cosmos/render/UniformContext.hpp>
#include <cosmos/render/UniformPresets.hpp>
#include <cosmos/assets/ShaderLibrary.hpp>

namespace cosmos::assets {

class MaterialLibrary {
public:
    static bool file_exists(const std::string& path) {
        return std::filesystem::exists(path);
    }
    static void add(const std::string& name, std::shared_ptr<render::PBRMaterial> material) {
        materials_[name] = material;
    }

    static std::shared_ptr<render::PBRMaterial> get(const std::string& name) {
        return materials_[name];
    }

    static void load_from_path(const std::string& name, const std::string& path) {
        auto material = std::make_shared<render::PBRMaterial>(ShaderLibrary::get("PBR"));
        material->bind_uniforms = render::UniformPresets::pbr_bind;

       
        if(file_exists(path + "/albedo.png")) {
            material->albedoMap = std::make_shared<render::Texture>(path + "/albedo.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        }
        if(file_exists(path + "/roughness.png")) {
            material->roughnessMap = std::make_shared<render::Texture>(path + "/roughness.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        }
        if(file_exists(path + "/metalness.png")) {
            material->metallicMap = std::make_shared<render::Texture>(path + "/metalness.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        }
        if(file_exists(path + "/ao.png")) {
            material->aoMap = std::make_shared<render::Texture>(path + "/ao.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        }
        if(file_exists(path + "/normal.png")) {
            material->normalMap = std::make_shared<render::Texture>(path + "/normal.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        }
        if(file_exists(path + "/displacement.png")) {
            material->displacementMap = std::make_shared<render::Texture>(path + "/displacement.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        }

        add(name, material);
    }


private:
    static inline std::unordered_map<std::string, std::shared_ptr<render::PBRMaterial>> materials_;
};

}
