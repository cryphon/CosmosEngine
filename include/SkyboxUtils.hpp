#pragma once

#include <glad/glad.h>
#include <memory>

class Shader;

GLuint equirectangular_to_cubemap(GLuint hdr_texture, std::shared_ptr<Shader> capture_shader, GLuint capture_fbo, GLuint capture_rbo);
float skybox_vertices[108];

