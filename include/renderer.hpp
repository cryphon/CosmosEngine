#pragma once

#include "shader.hpp"
#include "texture.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include <glm/glm.hpp>

class Renderer {
public:
    Renderer();
    ~Renderer();

    void initialize(); 
    void render(float scale, float rotation, glm::vec3 camPos, int screenWidth, int screenHeight);

private:
    Shader shader;
    Texture texture;
    VAO vao;
    VBO vbo;
    EBO ebo;
    GLuint uniID;
};

