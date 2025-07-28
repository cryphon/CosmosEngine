#include "Mesh.hpp"
#include "ObjLoader.hpp"
#include <iostream>


Mesh::Mesh() {
    draw_mode = MeshDrawMode::Indexed; 
    vertex_cnt = 0;
    index_cnt = 0;
}



void Mesh::init(const float* vertices, size_t v_size, const unsigned int* indices, size_t i_size) {
    index_cnt = i_size;

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

    std::cout << "index_cnt = " << index_cnt << "\n";
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
    if (!vbo) {
        std::cerr << "[Mesh::draw] Error: VBO not initialized\n";
        return;
    }

    if (vao.ID == 0) {
        std::cerr << "[Mesh::draw] Error: VAO not initialized\n";
        return;
    }

    vao.bind();

    switch (draw_mode) {
        case MeshDrawMode::Indexed:
            if (!ebo) {
                std::cerr << "[Mesh::draw] Error: EBO not initialized for Indexed draw mode\n";
                return;
            }
            if (index_cnt == 0) {
                std::cerr << "[Mesh::draw] Warning: index_cnt is 0\n";
                return;
            }
            glDrawElements(GL_TRIANGLES, index_cnt, GL_UNSIGNED_INT, nullptr);
            break;

        case MeshDrawMode::Arrays:
            if (vertex_cnt == 0) {
                std::cerr << "[Mesh::draw] Warning: vertex_cnt is 0\n";
                return;
            }
            glDrawArrays(GL_TRIANGLES, 0, vertex_cnt);
            break;

        default:
            std::cerr << "[Mesh::draw] Error: Invalid draw_mode\n";
            break;
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
    mesh->set_draw_mode(MeshDrawMode::Indexed);

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
