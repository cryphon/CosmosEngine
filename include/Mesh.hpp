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

struct Vec3 { float x, y, z; };
struct Vec2 { float u, v; };

struct Vertex {
    Vec3 position;
    Vec3 normal;
    Vec3 color;
    Vec2 texcoord;
    
    bool operator==(const Vertex& other) const {
        return position.x == other.position.x && position.y == other.position.y && position.z == other.position.z &&
               normal.x == other.normal.x && normal.y == other.normal.y && normal.z == other.normal.z &&
               texcoord.u == other.texcoord.u && texcoord.v == other.texcoord.v;
    }
};


template<>
    struct std::hash<Vertex> {
        size_t operator()(const Vertex& vertex) const {
            size_t h1 = hash<float>()(vertex.position.x);
            size_t h2 = hash<float>()(vertex.position.y);
            size_t h3 = hash<float>()(vertex.position.z);

            size_t h4 = hash<float>()(vertex.normal.x);
            size_t h5 = hash<float>()(vertex.normal.y);
            size_t h6 = hash<float>()(vertex.normal.z);

            size_t h7 = hash<float>()(vertex.texcoord.u);
            size_t h8 = hash<float>()(vertex.texcoord.v);

            // Combine hashes (boost-like hash combine)
            size_t seed = h1;
            seed ^= h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= h3 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= h4 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= h5 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= h6 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= h7 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= h8 + 0x9e3779b9 + (seed << 6) + (seed >> 2);

            return seed;
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
