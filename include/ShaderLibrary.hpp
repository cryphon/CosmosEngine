#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <functional>
#include "Shader.hpp"

struct ShaderPtrHash {
    std::size_t operator()(const std::shared_ptr<Shader>& s) const {
        return std::hash<Shader*>()(s.get());
    }
};

struct ShaderPtrEqual {
    bool operator()(const std::shared_ptr<Shader>& lhs, const std::shared_ptr<Shader>& rhs) const {
        return lhs.get() == rhs.get();
    }
};


class Shader;

class ShaderLibrary {
    public:
        static void load(const std::string& name, const std::string& vert_path, const std::string& frag_path);
        static std::shared_ptr<Shader> get(const std::string& name);
        static std::vector<std::string> get_keys();
        static std::string get_name(const std::shared_ptr<Shader>& shader);

    private:
        static std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
        static std::unordered_map<std::shared_ptr<Shader>, std::string, ShaderPtrHash, ShaderPtrEqual> reverse_lookup;
};
