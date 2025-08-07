#pragma once

#include <vector>
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include <memory>
#include <glm/glm.hpp>

enum class MeshDrawMode {
    Indexed,
    Arrays
};


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texcoord;
    glm::vec3 tangent;
    glm::vec3 bitangent;

    bool operator==(const Vertex& other) const {
        return position == other.position &&
        normal == other.normal &&
        texcoord == other.texcoord;
    }
};



template<>
struct std::hash<glm::vec2> {
    size_t operator()(const glm::vec2& v) const {
        return std::hash<float>()(v.x) ^ (std::hash<float>()(v.y) << 1);
    }
};

template<>
struct std::hash<glm::vec3> {
    size_t operator()(const glm::vec3& v) const {
        size_t h1 = std::hash<float>()(v.x);
        size_t h2 = std::hash<float>()(v.y);
        size_t h3 = std::hash<float>()(v.z);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

template<>
struct std::hash<Vertex> {
    size_t operator()(const Vertex& vertex) const {
        size_t h1 = std::hash<glm::vec3>()(vertex.position);
        size_t h2 = std::hash<glm::vec3>()(vertex.normal);
        size_t h3 = std::hash<glm::vec2>()(vertex.texcoord);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

class Mesh {
    public:
        Mesh();
        ~Mesh();


        // --- Mesh constructor for .obj files
        static std::unique_ptr<Mesh> from_obj(const std::string& path);

        // Setters
        void set_vertex_cnt(int v_cnt) { vertex_cnt = v_cnt; }
        void set_draw_mode(MeshDrawMode mode) { draw_mode = mode; }
        void init(const float* vertices, size_t v_size, const unsigned int* indices, size_t i_size);
        void init_positions_only(const float* vertices, size_t v_size);
        static std::unique_ptr<Mesh> create_uv_sphere(int segments, int rings, float radius = 1.0f);
        void draw() const;

        // Getters
        const std::vector<glm::vec3> get_vertices() const { return vbo->get_vertices(); }
        const std::vector<unsigned int> get_indices() const { return ebo->get_indices(); }

    private:
        VAO vao;
        std::unique_ptr<VBO> vbo;
        std::unique_ptr<EBO> ebo;
        GLsizei index_cnt = 0;
        int vertex_cnt = 0;
        MeshDrawMode draw_mode = MeshDrawMode::Indexed;
        
};
