// ==
// Standard Library
// ==
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <iostream>
#include <memory>

// ==
// Third Party
// ==
#include <glm/glm.hpp>

// ==
// Cosmos
// ==
#include <cosmos/assets/ObjLoader.hpp>
#include <cosmos/core/Logger.hpp>
#include <cosmos/render/Mesh.hpp>
#include <cosmos/render/HashUtils.hpp>

namespace cosmos::assets::ObjLoader {

std::shared_ptr<cosmos::render::Mesh> load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        LOG_ERROR(std::string("Failed to open OBJ file: ") += path);
        return nullptr;
    }

    std::vector<glm::vec3> temp_pos;
    std::vector<glm::vec2> temp_tex_coords;
    std::vector<glm::vec3> temp_normals;
    std::vector<Vertex> out_vertices;
    std::vector<uint32_t> out_indices;

    // --- Deduplication map ---
    std::unordered_map<Vertex, uint32_t> unique_vertices;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v") {
            glm::vec3 pos;
            ss >> pos.x >> pos.y >> pos.z;
            temp_pos.push_back(pos);
        } else if (type == "vt") {
            glm::vec2 uv;
            ss >> uv.x >> uv.y;
            temp_tex_coords.push_back(uv);
        } else if (type == "vn") {
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        } else if (type == "f") {
            // --- Face parsing; triangles, quads, n-gons
            std::vector<uint32_t> face_indices;

            std::string token;
            while (ss >> token) {
                int pIdx = 0, tIdx = 0, nIdx = 0;
                size_t slash1 = token.find('/');
                size_t slash2 = token.find('/', slash1 + 1);

                if (slash1 == std::string::npos) {
                    pIdx = std::stoi(token);
                } else if (slash2 == std::string::npos) {
                    pIdx = std::stoi(token.substr(0, slash1));
                    tIdx = std::stoi(token.substr(slash1 + 1));
                } else if (slash2 == slash1 + 1) {
                    pIdx = std::stoi(token.substr(0, slash1));
                    nIdx = std::stoi(token.substr(slash2 + 1));
                } else {
                    pIdx = std::stoi(token.substr(0, slash1));
                    tIdx = std::stoi(token.substr(slash1 + 1, slash2 - slash1 - 1));
                    nIdx = std::stoi(token.substr(slash2 + 1));
                }

                Vertex v{};
                if (pIdx > 0) v.position = temp_pos[pIdx - 1];
                if (tIdx > 0 && tIdx <= temp_tex_coords.size()) v.texcoord = temp_tex_coords[tIdx - 1];
                if (nIdx > 0 && nIdx <= temp_normals.size()) v.normal = temp_normals[nIdx - 1];

                auto it = unique_vertices.find(v);
                uint32_t index;
                if (it == unique_vertices.end()) {
                    index = static_cast<uint32_t>(out_vertices.size());
                    unique_vertices[v] = index;
                    out_vertices.push_back(v);
                } else {
                    index = it->second;
                }

                face_indices.push_back(index);
            }

            // Fan triangulation: convert n-gon to triangles
            for (size_t i = 1; i + 1 < face_indices.size(); ++i) {
                out_indices.push_back(face_indices[0]);
                out_indices.push_back(face_indices[i]);
                out_indices.push_back(face_indices[i + 1]);
            }
        }
    }

    std::vector<float> packed;
    packed.reserve(out_vertices.size() * 11);
    for (const auto& v : out_vertices) {
        packed.push_back(v.position.x);
        packed.push_back(v.position.y);
        packed.push_back(v.position.z);
        packed.push_back(v.normal.x);
        packed.push_back(v.normal.y);
        packed.push_back(v.normal.z);
        packed.push_back(1.0f); // default color R
        packed.push_back(1.0f); // default color G
        packed.push_back(1.0f); // default color B
        packed.push_back(v.texcoord.x);
        packed.push_back(v.texcoord.y);
    }

    auto mesh = std::make_unique<render::Mesh>();
    mesh->init(packed.data(), packed.size() * sizeof(float),
            out_indices.data(), out_indices.size() * sizeof(uint32_t));
    return mesh;
}
}
