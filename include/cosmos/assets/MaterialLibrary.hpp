#pragma once
// ==
// Standard Library
// ==
#include <unordered_map>
#include <memory>
#include <filesystem>
#include <mutex>

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

    // Registration-only (no file IO).
    static void load_from_path(const std::string& name, const std::string& path) {
        std::scoped_lock lk(mutex_);
        recipes_[name] = Recipe{path};
        // Do not create textures/materials here anymore.
    }

    // One-liner for call sites that don't want a separate register step.
    static std::shared_ptr<render::PBRMaterial>
    get_or_load_from_path(const std::string& name, const std::string& path) {
        {
            std::scoped_lock lk(mutex_);
            if (!recipes_.count(name)) recipes_[name] = Recipe{path};
        }
        return get(name);
    }

    // Fetch the material. If not instantiated yet, build it now (lazy).
    static std::shared_ptr<render::PBRMaterial> get(const std::string& name) {
        std::scoped_lock lk(mutex_);

        // Cache hit?
        if (auto it = cache_.find(name); it != cache_.end()) {
            if (auto sp = it->second.lock()) return sp;
        }

        // Need a recipe to build
        auto itR = recipes_.find(name);
        if (itR == recipes_.end())
            throw std::runtime_error("Material not registered: " + name);

        const Recipe& r = itR->second;

        // Build material (create once)
        // If you add ShaderLibrary::get_or_create, prefer that here.
        auto mat = std::make_shared<render::PBRMaterial>(ShaderLibrary::get_or_create("PBR", "shaders/pbr.vert", "shaders/pbr.frag"));
        mat->bind_uniforms = render::UniformPresets::full_lighting_bind;

        const std::string& path = r.path;
        if (file_exists(path + "/albedo.png")) {
            mat->albedoMap = std::make_shared<render::Texture>(path + "/albedo.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        }
        if (file_exists(path + "/roughness.png")) {
            mat->roughnessMap = std::make_shared<render::Texture>(path + "/roughness.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        }
        if (file_exists(path + "/metalness.png")) {
            mat->metallicMap = std::make_shared<render::Texture>(path + "/metalness.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        }
        if (file_exists(path + "/ao.png")) {
            mat->aoMap = std::make_shared<render::Texture>(path + "/ao.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        }
        if (file_exists(path + "/normal.png")) {
            mat->normalMap = std::make_shared<render::Texture>(path + "/normal.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        }
        if (file_exists(path + "/displacement.png")) {
            mat->displacementMap = std::make_shared<render::Texture>(path + "/displacement.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        }

        cache_[name] = mat;
        return mat;
    }

    // Optional: predicate
    static bool registered(const std::string& name) {
        std::scoped_lock lk(mutex_);
        return recipes_.count(name) > 0;
    }

private:
    struct Recipe {
        std::string path;
    };

    // name -> weak cache of instantiated materials
    static inline std::unordered_map<std::string, std::weak_ptr<render::PBRMaterial>> cache_;

    // name -> build recipe (path, etc.)
    static inline std::unordered_map<std::string, Recipe> recipes_;

    static inline std::mutex mutex_;
};
}
