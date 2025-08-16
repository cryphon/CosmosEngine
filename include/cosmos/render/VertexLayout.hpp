#pragma once
// ==
// Standard Library
// ==
#include <vector>
#include <array>

// ==
// Third Party
// ==
#include <glad/glad.h>

// ==
// Cosmos
// ==

namespace cosmos::render {

struct VertexAttribute {
    uint32_t index;       // shader location
    uint32_t size;         // num. components
    GLenum type;        // GL_FLOAT, etc...
    size_t offset;      // byte offset from start
};

inline bool operator==(const cosmos::render::VertexAttribute& a,
                       const cosmos::render::VertexAttribute& b) noexcept {
    return a.index  == b.index
        && a.size   == b.size
        && a.type   == b.type
        && a.offset == b.offset;
}

struct VertexLayoutDesc {
    GLsizei stride{};
    std::vector<VertexAttribute> attributes;

    bool operator==(const VertexLayoutDesc& o) const noexcept {
        return stride == o.stride && attributes == o.attributes;
    }
};

struct VertexLayoutDescHash {
    size_t operator()(const VertexLayoutDesc& d) const noexcept {
        size_t h = std::hash<GLsizei>{}(d.stride);
        auto mix = [&](size_t v){ h ^= v + 0x9e3779b97f4a7c15ULL + (h<<6) + (h>>2); };
        for (auto& a : d.attributes) {
            mix(std::hash<GLuint>{}(a.index));
            mix(std::hash<GLint>{}(a.size));
            mix(std::hash<GLenum>{}(a.type));
            mix(std::hash<size_t>{}(a.offset));
        }
        return h;
    }
};

// Non-templated "span-like" view used by the API.
struct VertexLayoutView {
    GLsizei               stride{};
    const VertexAttribute* attributes{nullptr};
    std::size_t           count{0};

    // iteration support (range-for)
    constexpr const VertexAttribute* begin() const { return attributes; }
    constexpr const VertexAttribute* end()   const { return attributes + count; }

    // convenience
    constexpr bool        empty() const { return count == 0; }
    constexpr std::size_t size()  const { return count; }

    // hashing (doesn't need to be constexpr)
    std::size_t hash() const {
        std::size_t h = std::hash<GLsizei>{}(stride);
        for (std::size_t i = 0; i < count; ++i) {
            const auto& a = attributes[i];
            // simple hash-combine
            auto mix = [&](std::size_t v) {
                h ^= v + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
            };
            mix(std::hash<GLuint>{}(a.index));
            mix(std::hash<GLint>{}(a.size));
            mix(std::hash<GLenum>{}(a.type));
            mix(std::hash<std::size_t>{}(a.offset));
        }
        return h;
    }
};

}


namespace std {
template<>
struct hash<cosmos::render::VertexLayoutView> {
    size_t operator()(const cosmos::render::VertexLayoutView& v) const noexcept {
        return v.hash();
    }
};
} // namespace std

namespace cosmos::render {

template <std::size_t N>
struct VertexLayout {
    GLsizei stride;
    std::array<VertexAttribute, N> attributes;

    constexpr operator VertexLayoutView() const {
        return { stride, attributes.data(), N };
    }
};
}
