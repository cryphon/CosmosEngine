#pragma once

#include "shader.hpp"
#include "texture.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include <glm/glm.hpp>
#include <memory>

class Renderer {
public:
    Renderer();
    ~Renderer();

    void initialize(); 
    void render(float scale, float rotation, glm::vec3 camPos, int screenWidth, int screenHeight);

private:
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Texture> texture;
    VAO vao;
    std::unique_ptr<VBO> vbo;
    std::unique_ptr<EBO> ebo;
    GLuint uniID;
};

