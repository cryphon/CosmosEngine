#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include <glm/glm.hpp>
#include <memory>
#include "Camera.hpp"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void initialize(); 
    void render(float scale, float rotation, const Camera& camera, int screenWidth, int screenHeight);

private:
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Texture> texture;
    VAO vao;
    std::unique_ptr<VBO> vbo;
    std::unique_ptr<EBO> ebo;
    GLuint uniID;
};

