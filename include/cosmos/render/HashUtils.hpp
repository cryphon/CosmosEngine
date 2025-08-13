#pragma once
#include <glm/glm.hpp>
#include <functional> 

// Forward declare your Vertex struct before using it here
struct Vertex;

namespace std {

template<>
struct hash<glm::vec2> {
    size_t operator()(const glm::vec2& v) const noexcept {
        return hash<float>()(v.x) ^ (hash<float>()(v.y) << 1);
    }
};

template<>
struct hash<glm::vec3> {
    size_t operator()(const glm::vec3& v) const noexcept {
        size_t h1 = hash<float>()(v.x);
        size_t h2 = hash<float>()(v.y);
        size_t h3 = hash<float>()(v.z);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

template<>
struct hash<Vertex> {
    size_t operator()(const Vertex& vertex) const noexcept {
        size_t h1 = hash<glm::vec3>()(vertex.position);
        size_t h2 = hash<glm::vec3>()(vertex.normal);
        size_t h3 = hash<glm::vec2>()(vertex.texcoord);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

} // namespace std

