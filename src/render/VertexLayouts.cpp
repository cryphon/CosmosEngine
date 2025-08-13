// ==
// Standard Library
// ==

// ==
// Third Party
// ==


// ==
// Cosmos
// ==
#include <cosmos/render/VertexLayout.hpp>

namespace cosmos::render::VertexLayouts {

const VertexLayout PBR = {
    17 * sizeof(float),
    {
        {0, 3, GL_FLOAT, 0},
        {1, 3, GL_FLOAT, 3 * sizeof(float)},
        {2, 3, GL_FLOAT, 6 * sizeof(float)},
        {3, 2, GL_FLOAT, 9 * sizeof(float)},
        {4, 3, GL_FLOAT, 11 * sizeof(float)},
        {5, 3, GL_FLOAT, 14 * sizeof(float)}
    }
};

const VertexLayout BasicOBJ = {
    11 * sizeof(float),
    {
        {0, 3, GL_FLOAT, 0},
        {1, 3, GL_FLOAT, 3 * sizeof(float)},
        {2, 3, GL_FLOAT, 6 * sizeof(float)},
        {3, 2, GL_FLOAT, 9 * sizeof(float)}
    }
};

const VertexLayout PositionOnly = {
    3 * sizeof(float),
    {
        {0, 3, GL_FLOAT, 0}
    }
};

}
