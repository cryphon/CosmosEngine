#include "Mesh.hpp"
#include "ObjLoader.hpp"
#include <iostream>
#include "Logger.hpp"
#include <glm/gtc/constants.hpp>


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


    LOG_DEBUG(std::string("index_cnt: ") += std::to_string(index_cnt));
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
        LOG_ERROR("[Mesh::draw] Error: VBO not initialized");
        return;
    }

    if (vao.ID == 0) {
        LOG_ERROR("[Mesh::draw] Error: VAO not initialized");
        return;
    }

    vao.bind();

    switch (draw_mode) {
        case MeshDrawMode::Indexed:
            if (!ebo) {
                LOG_ERROR("[Mesh::draw] Error: EBO not initialized for Indexed draw mode");
                return;
            }
            if (index_cnt == 0) {
                LOG_DEBUG("[Mesh::draw] Warning: index_cnt is 0");
                return;
            }
            glDrawElements(GL_TRIANGLES, index_cnt, GL_UNSIGNED_INT, nullptr);
            break;

        case MeshDrawMode::Arrays:
            if (vertex_cnt == 0) {
                LOG_DEBUG("[Mesh::draw] Warning: vertex_cnt is 0");
                return;
            }
            glDrawArrays(GL_TRIANGLES, 0, vertex_cnt);
            break;

        default:
            LOG_ERROR("[Mesh::draw] Error: Invalid draw_mode");
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
        LOG_ERROR(std::string("Failed to load OBJ: ") += path);
    }

    std::vector<float> packed;
    flatten_vbuf(vertices, packed);

    mesh->init(packed.data(), packed.size() * sizeof(float), indices.data(), indices.size() * sizeof(uint32_t)); 
    return mesh;
};


std::unique_ptr<Mesh> Mesh::create_uv_sphere(int segments, int rings, float radius) {
    auto mesh = std::make_unique<Mesh>();

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (int y = 0; y <= rings; ++y) {
        float v = static_cast<float>(y) / rings;
        float theta = v * glm::pi<float>();

        for (int x = 0; x <= segments; ++x) {
            float u = static_cast<float>(x) / segments;
            float phi = u * glm::two_pi<float>();

            float sinTheta = sin(theta);
            float cosTheta = cos(theta);
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            glm::vec3 pos = glm::vec3(
                radius * sinTheta * cosPhi,
                radius * cosTheta,
                radius * sinTheta * sinPhi
            );
            glm::vec3 normal = glm::normalize(pos);
            glm::vec2 texcoord = glm::vec2(u, 1.0f - v);

            vertices.push_back({ { pos.x, pos.y, pos.z }, { normal.x, normal.y, normal.z }, { texcoord.x, texcoord.y } });
        }
    }

    for (int y = 0; y < rings; ++y) {
        for (int x = 0; x < segments; ++x) {
            int i0 = y * (segments + 1) + x;
            int i1 = i0 + segments + 1;

            indices.push_back(i0);
            indices.push_back(i1);
            indices.push_back(i0 + 1);

            indices.push_back(i0 + 1);
            indices.push_back(i1);
            indices.push_back(i1 + 1);
        }
    }

    std::vector<float> packed;
    flatten_vbuf(vertices, packed);

    mesh->set_draw_mode(MeshDrawMode::Indexed);
    mesh->init(packed.data(), packed.size() * sizeof(float), indices.data(), indices.size() * sizeof(uint32_t));
    return mesh;
}


Mesh::~Mesh() {
    vao.delete_vao();
    if (vbo) vbo->delete_vbo();
    if (ebo) ebo->delete_ebo();
}
