#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"


#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 1000

GLfloat vertices[] = 
{
    -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,     // Lower left
    0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,     // Lower right
    0.0f, 0.5f * float(sqrt(3)) * 2/ 3, 0.0f,   // Upper
    -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,  // Inner left
    0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,  // Inner right
    0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f,     // Inner down
};

GLuint indices[] = 
{
    0, 3, 5,    // Lower left triangle
    3, 2, 4,    // Lower right triangle    
    5, 4, 1,    // Upper triangle
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

    VAO1.link_attr(VBO1, 0);
    VAO1.unbind();
    VBO1.unbind();
    EBO1.unbind();
        
    while(!glfwWindowShouldClose(window)) {

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_program.activate_shader();
        VAO1.bind();

        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    VAO1.delete_vao();
    VBO1.delete_vbo();
    EBO1.delete_ebo();
    shader_program.delete_shader();

    return 0;
}




