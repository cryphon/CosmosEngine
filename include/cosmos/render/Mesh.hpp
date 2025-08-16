#pragma once
// ==
// Standard Library
// ==
#include <vector>
#include <memory>
#include <unordered_map>

// ==
// Third Party
// ==
#include <glm/glm.hpp>

// ==
// Cosmos
// ==
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"


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

namespace cosmos::render {

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
        static std::unique_ptr<Mesh> create_uv_sphere(int segments, int rings, float radius = 1.0f, float tile = 1.0f);
        const VAO& vao_for(const VertexLayoutView& layout) const;
        void draw(const VertexLayoutView& layout) const;

        // Getters
        const std::vector<glm::vec3> get_vertices() const { return vbo->get_vertices(); }
        const std::vector<unsigned int> get_indices() const { return ebo->get_indices(); }

    private:
        std::unique_ptr<VBO> vbo;
        std::unique_ptr<EBO> ebo;
        mutable std::unordered_map<VertexLayoutDesc, VAO, VertexLayoutDescHash> vao_cache;

        GLsizei index_cnt = 0;
        int vertex_cnt = 0;
        MeshDrawMode draw_mode = MeshDrawMode::Indexed;
        
};
}
