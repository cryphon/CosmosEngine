#pragma once
// ==
// Standard Library
// ==

// ==
// Third Party
// ==


// ==
// Cosmos
// ==
#include <cosmos/assets/ResourceManager.hpp>
#include <cosmos/render/Material.hpp>
#include <cosmos/render/Mesh.hpp>
#include <cosmos/assets/ObjLoader.hpp>
#include <cosmos/assets/ShaderLibrary.hpp>
#include <cosmos/assets/MaterialLibrary.hpp>

namespace cosmos::assets {

inline std::shared_ptr<render::Shader>
load_shader(const std::string& name,
            const std::string& vert_path,
            const std::string& frag_path)
{
    // Use ShaderLibrary as backing store
    return ShaderLibrary::get_or_create(name, vert_path, frag_path);
}


inline std::shared_ptr<render::Material>
load_material_from_path(const std::string& name, const std::string& path)
{
    // Will register a recipe if not present, and build lazily.
    return MaterialLibrary::get_or_load_from_path(name, path);
}

inline std::shared_ptr<render::Material>
load_registered_material(const std::string& name)
{
    return MaterialLibrary::get(name);
}

inline std::shared_ptr<render::Mesh>
load_mesh(const std::string& path)
{
    return ObjLoader::load(path);
}

}
