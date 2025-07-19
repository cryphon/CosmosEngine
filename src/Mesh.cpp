#include "Mesh.hpp"

void Mesh::init(const float* vertices, size_t v_size, const unsigned int* indices, size_t i_size) {
    index_cnt = i_size / sizeof(unsigned int);

    vao.create();
    vao.bind();

    vbo = std::make_unique<VBO>(vertices, v_size);
    ebo = std::make_unique<EBO>(indices, i_size);

    vao.link_attr(*vbo, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);                      // position
    vao.link_attr(*vbo, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));    // normal
    vao.link_attr(*vbo, 2, 3, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));    // color
    vao.link_attr(*vbo, 3, 2, GL_FLOAT, 11 * sizeof(float), (void*)(9 * sizeof(float)));    // texCoord
    vao.unbind();
    vbo->unbind();
    ebo->unbind();
}

void Mesh::init_positions_only(const float* vertices, size_t v_size) {
    index_cnt = 36; // no EBO
    vao.create();
    vao.bind();

    vbo = std::make_unique<VBO>(vertices, v_size);
    vao.link_attr(*vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0); // only position
    
    vao.unbind();
    vbo->unbind();
}

void Mesh::draw() const {
    if (vao.ID == 0 || index_cnt == 0) return;  // skip if uninitialized
    vao.bind();
    vao.bind();
    if (draw_mode == MeshDrawMode::Indexed) {
        glDrawElements(GL_TRIANGLES, index_cnt, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, vertex_cnt);
    }
}

Mesh::~Mesh() {
    vao.delete_vao();
    if (vbo) vbo->delete_vbo();
    if (ebo) ebo->delete_ebo();
}
