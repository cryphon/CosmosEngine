#pragma once


#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include <memory>

class Mesh {
    public:
        Mesh() = default;
        ~Mesh();

        void init(const float* vertices, size_t v_size, const unsigned int* indices, size_t i_size);
        void draw() const;
    private:
        VAO vao;
        std::unique_ptr<VBO> vbo;
        std::unique_ptr<EBO> ebo;
        GLsizei index_cnt = 0;
};
