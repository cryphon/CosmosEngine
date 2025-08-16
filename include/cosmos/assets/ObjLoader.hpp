#pragma once
// ==
// Standard Library
// ==
#include <string>
#include <vector>
#include <memory>

// ==
// Third Party
// ==

// ==
// Cosmos
// ==
#include <cosmos/render/VertexLayout.hpp>

// ==
// Forward Declare
// ==
namespace cosmos::render { class Mesh; }

namespace  cosmos::assets::ObjLoader {
    std::shared_ptr<render::Mesh> load(const std::string& path);
}
