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

// ==
// Forward Declare
// ==
#include <cosmos/render/VertexLayout.hpp>

namespace cosmos::render::VertexLayouts {

static constexpr VertexLayout<6> PBR {
    17 * sizeof(float),
    {{
        {0,3,GL_FLOAT,0},
        {1,3,GL_FLOAT,3*sizeof(float)},
        {2,3,GL_FLOAT,6*sizeof(float)},
        {3,2,GL_FLOAT,9*sizeof(float)},
        {4,3,GL_FLOAT,11*sizeof(float)},
        {5,3,GL_FLOAT,14*sizeof(float)}
    }}
};

static constexpr VertexLayout<4> BasicOBJ {
    11 * sizeof(float),
    {{
        {0,3,GL_FLOAT,0},
        {1,3,GL_FLOAT,3*sizeof(float)},
        {2,3,GL_FLOAT,6*sizeof(float)},
        {3,2,GL_FLOAT,9*sizeof(float)}
    }}
};

static constexpr VertexLayout<1> PositionOnly {
    3 * sizeof(float),
    {{
        {0,3,GL_FLOAT,0}
    }}
};

}
