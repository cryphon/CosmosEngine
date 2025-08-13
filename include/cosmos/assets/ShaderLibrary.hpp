#pragma once
// ==
// Standard Library
// ==
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <functional>

// ==
// Third Party
// ==

// ==
// Cosmos
// ==
#include <cosmos/render/Shader.hpp>

namespace cosmos::assets {
struct ShaderPtrHash {
    std::size_t operator()(const std::shared_ptr<render::Shader>& s) const {
        return std::hash<render::Shader*>()(s.get());
    }
};

struct ShaderPtrEqual {
    bool operator()(const std::shared_ptr<render::Shader>& lhs, const std::shared_ptr<render::Shader>& rhs) const {
        return lhs.get() == rhs.get();
    }
};


class Shader;

class ShaderLibrary {
    public:
        static void load(const std::string& name, const std::string& vert_path, const std::string& frag_path);
        static std::shared_ptr<render::Shader> get(const std::string& name);
        static std::vector<std::string> get_keys();
        static std::string get_name(const std::shared_ptr<render::Shader>& shader);

    private:
        static std::unordered_map<std::string, std::shared_ptr<render::Shader>> shaders;
        static std::unordered_map<std::shared_ptr<render::Shader>, std::string, ShaderPtrHash, ShaderPtrEqual> reverse_lookup;
};
}
