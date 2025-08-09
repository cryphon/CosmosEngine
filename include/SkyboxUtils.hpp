#pragma once

#include <glad/glad.h>
#include <memory>

class SkyBox;
class Shader;
class SkyBoxManager;

GLuint equirectangular_to_cubemap(GLuint hdr_texture, std::shared_ptr<Shader> capture_shader, GLuint capture_fbo, GLuint capture_rbo);
std::shared_ptr<SkyBox> init_hdri_skybox(const std::shared_ptr<SkyBoxManager> skybox_manager, const std::string name, const std::string& hdr_path);
extern float skybox_vertices[108];

