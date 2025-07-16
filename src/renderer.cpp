#include "renderer.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static GLfloat vertices[] = {
    -0.5f, 0.0f,  0.5f,  0.83f, 0.70f, 0.44f, 0.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,  0.83f, 0.70f, 0.44f, 5.0f, 0.0f,
     0.5f, 0.0f, -0.5f,  0.83f, 0.70f, 0.44f, 0.0f, 0.0f,
     0.5f, 0.0f,  0.5f,  0.83f, 0.70f, 0.44f, 5.0f, 0.0f,
     0.0f, 0.8f,  0.0f,  0.92f, 0.86f, 0.76f, 2.5f, 5.0f
};

static GLuint indices[] = {
    0, 1, 2,
    0, 2, 3,
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4
};

Renderer::Renderer() {}

Renderer::~Renderer() {
    vao.delete_vao();
    if (vbo) vbo->delete_vbo();
    if (ebo) ebo->delete_ebo();
    if (texture) texture->delete_texture();
    if (shader) shader->delete_shader();
}

void Renderer::initialize() {
    shader = std::make_unique<Shader>("shaders/default.vert", "shaders/default.frag");
    texture = std::make_unique<Texture>("pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

    vao.create();
    vao.bind();
    vbo = std::make_unique<VBO>(vertices, sizeof(vertices));
    ebo = std::make_unique<EBO>(indices, sizeof(indices));

    vao.link_attr(*vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    vao.link_attr(*vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    vao.link_attr(*vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    vao.unbind(); vbo->unbind(); ebo->unbind();

    texture->tex_unit(*shader, "tex0", 0);
    uniID = glGetUniformLocation(shader->ID, "scale");
}

void Renderer::render(float scale, float rotation, glm::vec3 camPos, int width, int height) {
    shader->activate_shader();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(scale));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 view = glm::translate(glm::mat4(1.0f), camPos);
    glm::mat4 proj = glm::perspective(glm::radians(60.0f), (float)width / height, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

    glUniform1f(uniID, 0.5f);
    texture->bind();
    vao.bind();

    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
}
