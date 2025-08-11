#pragma once
#include <string>
#include <vector>
#include <memory>
#include "VertexLayout.hpp"

class Mesh;

struct MeshData {
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    VertexLayout layout;
};

namespace  ObjLoader {
    std::shared_ptr<Mesh> load(const std::string& path);
}
