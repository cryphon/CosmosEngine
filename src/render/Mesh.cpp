// ==
// Standard Library
// ==
#include <iostream>

// ==
// Third Party
// ==
#include <glm/gtc/constants.hpp>

// ==
// Cosmos
// ==
#include <cosmos/render/Mesh.hpp>
#include <cosmos/render/VertexLayout.hpp>
#include <cosmos/assets/ObjLoader.hpp>
#include <cosmos/core/Logger.hpp>

namespace cosmos::render {


Mesh::Mesh() {
    draw_mode = MeshDrawMode::Indexed; 
    vertex_cnt = 0;
    index_cnt = 0;
}



void Mesh::init(const float* vertices, size_t v_size,
                const unsigned int* indices, size_t i_size) {
    index_cnt = static_cast<size_t>(i_size / sizeof(unsigned int));
    vertex_cnt = static_cast<size_t>(v_size / sizeof(float)); // Not exact verts, just float count

    // Create buffers but DO NOT bind attributes yet
    vbo = std::make_unique<VBO>(vertices, v_size);
    if (i_size > 0) {
        ebo = std::make_unique<EBO>(indices, i_size);
    }

    vao_cache.clear(); // Reset per-layout VAOs
}


void Mesh::init_positions_only(const float* vertices, size_t v_size) {
    vertex_cnt = static_cast<size_t>(v_size / sizeof(float) / 3);
    index_cnt = 0; // No EBO

    vbo = std::make_unique<VBO>(vertices, v_size);
    ebo.reset(); // No index buffer

    vao_cache.clear();
}


void Mesh::draw(const VertexLayoutView& layout) const { 
    if (!vbo) { LOG_ERROR("[Mesh::draw] VBO not initialized"); return; }

    const VAO& vao = vao_for(layout);
    vao.bind();

    switch (draw_mode) {
        case MeshDrawMode::Indexed:
            if (!ebo) { LOG_ERROR("[Mesh::draw] EBO not initialized for Indexed"); vao.unbind(); return; }
            if (index_cnt == 0) { LOG_DEBUG("[Mesh::draw] index_cnt == 0"); vao.unbind(); return; }
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(index_cnt), GL_UNSIGNED_INT, nullptr);
            break;
        case MeshDrawMode::Arrays:
            if (vertex_cnt == 0) { LOG_DEBUG("[Mesh::draw] vertex_cnt == 0"); vao.unbind(); return; }
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertex_cnt));
            break;
        default:
            LOG_ERROR("[Mesh::draw] Invalid draw_mode");
            break;
    }

    vao.unbind();
}


static void flatten_vbuf(const std::vector<Vertex> &verts, std::vector<float> &out_data) {
    out_data.clear();
    out_data.reserve(verts.size() * 17);

    for(const auto& v : verts) {
        out_data.push_back(v.position.x);
        out_data.push_back(v.position.y);
        out_data.push_back(v.position.z);
        out_data.push_back(v.normal.x);
        out_data.push_back(v.normal.y);
        out_data.push_back(v.normal.z);
        out_data.push_back(v.color.x); // default white color R
        out_data.push_back(v.color.y); // default white color G
        out_data.push_back(v.color.z); // default white color B
        out_data.push_back(v.texcoord.x);
        out_data.push_back(v.texcoord.y);

        out_data.push_back(v.tangent.x);
        out_data.push_back(v.tangent.y);
        out_data.push_back(v.tangent.z);
        out_data.push_back(v.bitangent.x);
        out_data.push_back(v.bitangent.y);
        out_data.push_back(v.bitangent.z);


    }
}

std::unique_ptr<Mesh> Mesh::from_obj(const std::string& path) {
    auto mesh = std::make_unique<Mesh>();
    mesh->set_draw_mode(MeshDrawMode::Indexed);

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    if(!assets::ObjLoader::load(path)) {
        LOG_ERROR(std::string("Failed to load OBJ: ") += path);
    }


    std::vector<float> packed;
    flatten_vbuf(vertices, packed);

    mesh->init(packed.data(), packed.size() * sizeof(float), indices.data(), indices.size() * sizeof(uint32_t)); 
    return mesh;
};


std::unique_ptr<Mesh> Mesh::create_uv_sphere(int segments, int rings, float radius, float tile) {
    auto mesh = std::make_unique<Mesh>();

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    if (!vertices.empty()) {
        std::cout << "First texcoord: (" 
            << vertices[0].texcoord.x << ", " 
            << vertices[0].texcoord.y << ")\n";
    };
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
            glm::vec3 color = glm::vec3(1.0f); // unused but required
            glm::vec2 texcoord = glm::vec2(u * tile, v * tile);

            vertices.push_back({ 
                { pos.x, pos.y, pos.z },
                { normal.x, normal.y, normal.z },
                { color.x, color.y, color.z },
                { texcoord.x, texcoord.y }
            });
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

    for (size_t i = 0; i < indices.size(); i += 3) {
        Vertex& v0 = vertices[indices[i + 0]];
        Vertex& v1 = vertices[indices[i + 1]];
        Vertex& v2 = vertices[indices[i + 2]];

        glm::vec3 edge1 = v1.position - v0.position;
        glm::vec3 edge2 = v2.position - v0.position;
        glm::vec2 deltaUV1 = v1.texcoord - v0.texcoord;
        glm::vec2 deltaUV2 = v2.texcoord - v0.texcoord;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent = f * (deltaUV2.y * edge1 - deltaUV1.y * edge2);
        glm::vec3 bitangent = f * (-deltaUV2.x * edge1 + deltaUV1.x * edge2);

        v0.tangent += tangent;
        v1.tangent += tangent;
        v2.tangent += tangent;

        v0.bitangent += bitangent;
        v1.bitangent += bitangent;
        v2.bitangent += bitangent;
    }

    // Normalize tangents and bitangents
    for (auto& v : vertices) {
        v.tangent = glm::normalize(v.tangent);
        v.bitangent = glm::normalize(v.bitangent);
    }


    std::vector<float> packed;
    flatten_vbuf(vertices, packed);

    mesh->set_draw_mode(MeshDrawMode::Indexed);
    mesh->init(packed.data(), packed.size() * sizeof(float), indices.data(), indices.size() * sizeof(uint32_t));
    return mesh;
}

const VAO& Mesh::vao_for(const VertexLayoutView& layout) const {

    if (!layout.attributes) {
        LOG_ERROR("[vao_for] layout.attributes == nullptr!");
        throw std::runtime_error("Invalid VertexLayoutView: null attributes");
    }
    if (layout.count > 32) {
        LOG_ERROR("[vao_for] layout.count is insane: ");
        throw std::runtime_error("Invalid VertexLayoutView: count too large");
    }

    // Make an owning copy of the layout
    VertexLayoutDesc key{ layout.stride, { layout.begin(), layout.end() } };

    auto it = vao_cache.find(key);
    if (it != vao_cache.end())
        return it->second;

    VAO vao;
    vao.create();
    vao.bind();
    vbo->bind();
    if (ebo) ebo->bind();
    for (auto& a : key.attributes) {
        glEnableVertexAttribArray(a.index);
        glVertexAttribPointer(a.index, a.size, a.type, GL_FALSE, key.stride, (void*)a.offset);
    }
    vao.unbind();

    return vao_cache.emplace(std::move(key), std::move(vao)).first->second;
}


Mesh::~Mesh() { }

}
