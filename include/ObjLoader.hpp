#pragma once
#include <string>
#include <vector>
#include <memory>

class Mesh;

namespace  ObjLoader {
    std::shared_ptr<Mesh> load(const std::string& path);
}
