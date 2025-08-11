#pragma once
#include <glad/glad.h>
#include <vector>

struct VertexAttribute {
    GLuint index;       // shader location
    GLint size;         // num. components
    GLenum type;        // GL_FLOAT, etc...
    size_t offset;      // byte offset from start
};

struct VertexLayout {
    GLsizei stride;
    std::vector<VertexAttribute> attributes;

    size_t hash() const {
        size_t h = std::hash<GLsizei>()(stride);
        for (const auto& attr : attributes) {
            // Combine attribute properties into the hash
            h ^= std::hash<GLuint>()(attr.index) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= std::hash<GLint>()(attr.size) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= std::hash<GLenum>()(attr.type) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= std::hash<size_t>()(attr.offset) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
};
