#pragma once
// ==
// Standard Library
// ==
#include <string>

// ==
// Third Party
// ==
#include <glad/glad.h>

// ==
// Cosmos
// ==

namespace cosmos::render {
GLuint load_hdr_texture(const std::string& path);
}
