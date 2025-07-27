#include "Mesh.hpp"
#include "ObjLoader.hpp"
#include <iostream>

void Mesh::init(const float* vertices, size_t v_size, const unsigned int* indices, size_t i_size) {
    index_cnt = i_size / sizeof(unsigned int);

    vao.create();
    vao.bind();

    vbo = std::make_unique<VBO>(vertices, v_size);
    ebo = std::make_unique<EBO>(indices, i_size);

    this->vertices.clear();
    for (size_t i = 0; i < v_size / sizeof(float); i += 11) {
        this->vertices.push_back(glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]));
    }

    this->index_buffer.assign(indices, indices + index_cnt);

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

    this->vertices.clear();
    for (size_t i = 0; i < v_size / sizeof(float); i += 3) {
        this->vertices.push_back(glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]));
    }

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

static void flatten_vbuf(const std::vector<Vertex> &verts, std::vector<float> &out_data) {
    out_data.clear();
    out_data.reserve(verts.size() * 11);

    for(const auto& v : verts) {
        out_data.push_back(v.position.x);
        out_data.push_back(v.position.y);
        out_data.push_back(v.position.z);
        out_data.push_back(v.normal.x);
        out_data.push_back(v.normal.y);
        out_data.push_back(v.normal.z);
        out_data.push_back(1.0f); // default white color R
        out_data.push_back(1.0f); // default white color G
        out_data.push_back(1.0f); // default white color B
        out_data.push_back(v.texcoord.u);
        out_data.push_back(v.texcoord.v);
    }
}

std::unique_ptr<Mesh> Mesh::from_obj(const std::string& path) {
    auto mesh = std::make_unique<Mesh>();

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    if(!ObjLoader::load(path)) {
        std::cerr << "Failed to load OBJ: " << path << std::endl;
    }

    std::vector<float> packed;
    flatten_vbuf(vertices, packed);

    mesh->init(packed.data(), packed.size() * sizeof(float), indices.data(), indices.size() * sizeof(uint32_t)); 
    return mesh;
};


Mesh::~Mesh() {
    vao.delete_vao();
    if (vbo) vbo->delete_vbo();
    if (ebo) ebo->delete_ebo();
}
