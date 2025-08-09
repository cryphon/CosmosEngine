#pragma once
#include <unordered_map>
#include <memory>
#include <filesystem>
#include "PBRMaterial.hpp"
#include "UniformPresets.hpp"
#include "ShaderLibrary.hpp"





class MaterialLibrary {
public:
    static bool file_exists(const std::string& path) {
        return std::filesystem::exists(path);
    }
    static void add(const std::string& name, std::shared_ptr<PBRMaterial> material) {
        materials[name] = material;
    }

    static std::shared_ptr<PBRMaterial> get(const std::string& name) {
        return materials[name];
    }

    static void load_from_path(const std::string& name, const std::string& path) {
        auto material = std::make_shared<PBRMaterial>(ShaderLibrary::get("PBR"));
        material->bind_uniforms = UniformPresets::pbr_bind;

       
        if(file_exists(path + "/albedo.png")) {
            material->albedoMap = std::make_shared<Texture>(path + "/albedo.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        }
        if(file_exists(path + "/roughness.png")) {
            material->roughnessMap = std::make_shared<Texture>(path + "/roughness.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        }
        if(file_exists(path + "/metalness.png")) {
            material->metallicMap = std::make_shared<Texture>(path + "/metalness.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        }
        if(file_exists(path + "/ao.png")) {
            material->aoMap = std::make_shared<Texture>(path + "/ao.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        }
        if(file_exists(path + "/normal.png")) {
            material->normalMap = std::make_shared<Texture>(path + "/normal.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        }
        if(file_exists(path + "/displacement.png")) {
            material->displacementMap = std::make_shared<Texture>(path + "/displacement.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        }

        add(name, material);
    }


private:
    static inline std::unordered_map<std::string, std::shared_ptr<PBRMaterial>> materials;
};
