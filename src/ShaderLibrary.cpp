#include "ShaderLibrary.hpp"

std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderLibrary::shaders;

std::unordered_map<
    std::shared_ptr<Shader>,
    std::string,
    ShaderPtrHash,
    ShaderPtrEqual
> ShaderLibrary::reverse_lookup;


void ShaderLibrary::load(const std::string& name, const std::string& vert_path, const std::string& frag_path) {
    auto shader = std::make_shared<Shader>(vert_path.c_str(), frag_path.c_str());
    shaders[name] = shader;
    reverse_lookup[shader] = name;
}

std::shared_ptr<Shader> ShaderLibrary::get(const std::string& name) {
    return shaders.at(name);
}

std::vector<std::string> ShaderLibrary::get_keys() {
    std::vector<std::string> keys;
    for (const auto& pair : shaders) {
        keys.push_back(pair.first);
    }
    return keys;
}

std::string ShaderLibrary::get_name(const std::shared_ptr<Shader>& shader) {
    auto it = reverse_lookup.find(shader);
    if (it != reverse_lookup.end()) {
        return it->second;
    }
    return "<unknown>";
}
