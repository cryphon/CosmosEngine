#pragma once
// ==
// Standard Library
// ==

// ==
// Third Party
// ==
#include <glad/glad.h>

// ==
// Cosmos
// ==

namespace cosmos::render::gfx {

struct BlendFunc { GLenum src = GL_SRC_ALPHA, dst = GL_ONE_MINUS_SRC_ALPHA; };

struct RenderState {
    bool blending = false;
    bool depth_test = true;
    bool depth_write = true;
    bool cull_face = true;
    GLenum cull_mode = GL_BACK;
    GLenum depth_func = GL_LESS;
    BlendFunc blend{GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};

    static RenderState Opaque() {
        RenderState s; /* defaults are fine */ return s;
    }
    static RenderState Transparent() {
        RenderState s;
        s.blending = true;
        s.depth_write = false;
        s.blend = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };
        return s;
    }
};
}
