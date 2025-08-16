// ==
// Standard Library
// ==

// ==
// Third Party
// ==


// ==
// Cosmos
// ==
#include <cosmos/assets/ShaderLibrary.hpp>

namespace cosmos::assets {
std::unordered_map<std::string, std::shared_ptr<render::Shader>> ShaderLibrary::shaders_;

std::unordered_map<
    std::shared_ptr<render::Shader>,
    std::string,
    ShaderPtrHash,
    ShaderPtrEqual
> ShaderLibrary::reverse_lookup_;


void ShaderLibrary::load(const std::string& name, const std::string& vert_path, const std::string& frag_path) {
    auto shader = std::make_shared<render::Shader>(vert_path.c_str(), frag_path.c_str());
    shaders_[name] = shader;
    reverse_lookup_[shader] = name;
}

std::shared_ptr<render::Shader> ShaderLibrary::get(const std::string& name) {
    return shaders_.at(name);
}

std::vector<std::string> ShaderLibrary::get_keys() {
    std::vector<std::string> keys;
    for (const auto& pair : shaders_) {
        keys.push_back(pair.first);
    }
    return keys;
}

std::string ShaderLibrary::get_name(const std::shared_ptr<render::Shader>& shader) {
    auto it = reverse_lookup_.find(shader);
    if (it != reverse_lookup_.end()) {
        return it->second;
    }
    return "<unknown>";
}
}
