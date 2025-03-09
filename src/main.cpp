#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.hpp"

#define SCREEN_HEIGHT 600
#define SCREEN_WIDTH 800

// camera
glm::vec3 cam_pos = glm::vec3(0.0f, 1.0f, 3.0f);
glm::vec3 cam_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cam_up = glm::vec3(0.0f, 1.0f, 0.0f);

float delta = 0.0f;
float last_frame = 0.0f;

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
    float cam_speed = 2.5 * delta;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam_pos += cam_speed * cam_front;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cam_pos -= cam_speed * cam_front;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cam_pos -= glm::normalize(glm::cross(cam_front, cam_up)) * cam_speed;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cam_pos += glm::normalize(glm::cross(cam_front, cam_up)) * cam_speed;
    }
}


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
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" <<std::endl;
        return -1;
    }


    // shader program
    Shader shader_program("shader.vs", "shader.fs");


    // enable depth testing
    glEnable(GL_DEPTH_TEST);


    float plane_vertices[] =  {
        -5.0f, 0.0f, -5.0f,
         5.0f, 0.0f, -5.0f,
         5.0f, 0.0f,  5.0f,
        -5.0f, 0.0f,  5.0f
    };

    unsigned int plane_indices[] = { 0, 1, 2, 2, 3, 0 };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), plane_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(plane_indices), plane_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

   // Main render loop
    while(!glfwWindowShouldClose(window)) {
        // Calculate delta time for smooth movement
        float curr_frame = glfwGetTime();
        delta = curr_frame - last_frame;
        last_frame = curr_frame;

        // Process user input
        process_input(window);

        // Clear screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);  // Dark background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader program
        shader_program.use();

        // Create and set transformation matrices
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(cam_pos, cam_pos + cam_front, cam_up);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 
                                               static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 
                                               0.1f, 100.0f);

        // Pass matrices to shader
        shader_program.setMat4("model", model);
        shader_program.setMat4("view", view);
        shader_program.setMat4("projection", projection);
        
        // Render the plane
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap buffers and poll for events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}




