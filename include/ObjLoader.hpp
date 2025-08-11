#pragma once
#include <string>
#include <vector>
#include <memory>
#include "VertexLayout.hpp"

class Mesh;

namespace  ObjLoader {
    std::shared_ptr<Mesh> load(const std::string& path);
}
