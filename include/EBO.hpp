#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>
#include <stddef.h>
#include <vector>

class EBO {
    public:
        GLuint ID;

        EBO(const unsigned int* indices, size_t size);
        void bind();
        void unbind();
        void delete_ebo();

        const std::vector<unsigned int>& get_indices() const { return index_data; };

    private:
        std::vector<unsigned int> index_data;
};

#endif
