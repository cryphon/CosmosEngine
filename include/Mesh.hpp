#pragma once


#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include <memory>

enum class MeshDrawMode {
    Indexed,
    Arrays
};

class Mesh {
    public:
        Mesh() = default;
        ~Mesh();

        // Setters
        void set_vertex_cnt(int v_cnt) { vertex_cnt = v_cnt; }
        void set_draw_mode(MeshDrawMode mode) { draw_mode = mode; }

        void init(const float* vertices, size_t v_size, const unsigned int* indices, size_t i_size);
        void init_positions_only(const float* vertices, size_t v_size);
        void draw() const;
    private:
        VAO vao;
        std::unique_ptr<VBO> vbo;
        std::unique_ptr<EBO> ebo;
        GLsizei index_cnt = 0;
        int vertex_cnt = 0;
        MeshDrawMode draw_mode = MeshDrawMode::Indexed;
        
};
