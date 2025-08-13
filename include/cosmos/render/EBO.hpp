#pragma once
// ==
// Standard Library
// ==
#include <stddef.h>
#include <vector>

// ==
// Third Party
// ==
#include <glad/glad.h>

// ==
// Cosmos
// ==

namespace cosmos::render {
class EBO {
    public:
        GLuint ID;

        EBO(const unsigned int* indices, size_t size);
        void bind();
        void unbind();
        void delete_ebo();

        const std::vector<unsigned int>& get_indices() const { return index_data_; };

    private:
        std::vector<unsigned int> index_data_;
};
}

