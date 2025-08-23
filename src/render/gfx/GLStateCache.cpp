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
#include <cosmos/render/gfx/GLStateCache.hpp>

namespace cosmos::render::gfx {

void GLStateCache::set_cap(GLenum cap, bool enable, bool& flag) {
    if(flag != enable) { (enable ? glEnable : glDisable)(cap); flag = enable; }
}

// GLStateCache.cpp
void GLStateCache::apply(const RenderState& s) {
    set_cap(GL_DEPTH_TEST, s.depth_test, cached_.depth_test);
    set_cap(GL_CULL_FACE,  s.cull_face,  cached_.cull_face);

    if (cached_.depth_write != s.depth_write) {
        glDepthMask(s.depth_write ? GL_TRUE : GL_FALSE);
        cached_.depth_write = s.depth_write;
    }
    if (s.cull_face && cached_.cull_mode != s.cull_mode) {
        glCullFace(s.cull_mode); cached_.cull_mode = s.cull_mode;
    }
    if (cached_.depth_func != s.depth_func) {
        glDepthFunc(s.depth_func); cached_.depth_func = s.depth_func;
    }

    // --- Handle blending explicitly (mirrors your manual fix) ---
    bool just_enabled = false;
    if (s.blending) {
        if (!cached_.blending) {
            glEnable(GL_BLEND);
            cached_.blending = true;
            just_enabled = true;
        }
        // Always ensure func is correct when blending is ON,
        // and especially right after enabling.
        if (just_enabled ||
            cached_.blend.src != s.blend.src ||
            cached_.blend.dst != s.blend.dst) {
            glBlendFunc(s.blend.src, s.blend.dst);
            cached_.blend = s.blend;
        }
    } else {
        if (cached_.blending) {
            glDisable(GL_BLEND);
            cached_.blending = false;
        }
    }
}
}
