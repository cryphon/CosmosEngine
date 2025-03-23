#include <iostream>
#include <glad/glad.h>
#include <stb/stb_image.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.hpp"
#include "texture.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"


#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 800

GLfloat vertices[] = 
{
    -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // Lower left
    -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,  0.0f, 1.0f, // Upper left
     0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Upper right
     0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,  1.0f, 0.0f, // Lower right
};
GLuint indices[] = 
{
    0, 2, 1,
    0, 3, 2,
};

int main(void) {
    if(!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // set Core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

 
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D Space", nullptr, nullptr);
    if(!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" <<std::endl;
        return -1;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    Shader shader_program("shaders/default.vert", "shaders/default.frag"); 

    VAO VAO1;
    VAO1.bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.link_attr(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.link_attr(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.link_attr(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    VAO1.unbind();
    VBO1.unbind();
    EBO1.unbind();

    GLuint uniID = glGetUniformLocation(shader_program.ID, "scale");

    Texture tex("pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    tex.tex_unit(shader_program, "tex0", 0);

    while(!glfwWindowShouldClose(window)) {

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_program.activate_shader();
        glUniform1f(uniID, 0.5f);


        tex.bind();
        VAO1.bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    VAO1.delete_vao();
    VBO1.delete_vbo();
    EBO1.delete_ebo();
    tex.delete_texture();
    shader_program.delete_shader();

    return 0;
}




