#pragma once
// ==
// Standard Library
// ==

// ==
// Third Party
// ==

// ==
// Cosmos
// ==
#include <cosmos/render/gfx/RenderState.hpp>

namespace cosmos::render::gfx {

class GLStateCache {
public:
    void apply(const RenderState& s);

private:
    RenderState cached_{};
    void set_cap(GLenum cap, bool enable, bool& flag);
};
}
