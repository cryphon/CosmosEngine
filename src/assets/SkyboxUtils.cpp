// ==
// Standard Library
// ==

// ==
// Third Party
// ==
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// ==
// Cosmos
// ==
#include <cosmos/assets/SkyboxUtils.hpp>
#include <cosmos/assets/SkyboxManager.hpp>
#include <cosmos/render/Mesh.hpp>
#include <cosmos/render/Shader.hpp>
#include <cosmos/render/Material.hpp>
#include <cosmos/render/TextureHDR.hpp>
#include <cosmos/render/Texture.hpp>
#include <cosmos/render/UniformPresets.hpp>
#include <cosmos/render/VertexLayouts.hpp>
#include <cosmos/core/Logger.hpp>

namespace cosmos::assets {

GLuint equirectangular_to_cubemap(GLuint hdr_texture, std::shared_ptr<render::Shader> capture_shader, GLuint capture_fbo, GLuint capture_rbo) {
    // Create output cubemap texture
    GLuint cubemap;
    const unsigned int cubemap_size = 1024;
    // --- Viewport
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);


    glGenTextures(1, &cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, cubemap_size, cubemap_size, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set up cube mesh
    auto cube = std::make_unique<render::Mesh>();
    cube->init_positions_only(skybox_vertices, sizeof(skybox_vertices));
    cube->set_vertex_cnt(36);
    cube->set_draw_mode(MeshDrawMode::Arrays);

    // Set up camera projections and views
    glm::mat4 capture_proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 capture_views[] = {
        glm::lookAt(glm::vec3(0,0,0), glm::vec3( 1, 0, 0), glm::vec3(0,-1, 0)),
        glm::lookAt(glm::vec3(0,0,0), glm::vec3(-1, 0, 0), glm::vec3(0,-1, 0)),
        glm::lookAt(glm::vec3(0,0,0), glm::vec3( 0, 1, 0), glm::vec3(0, 0, 1)),
        glm::lookAt(glm::vec3(0,0,0), glm::vec3( 0,-1, 0), glm::vec3(0, 0,-1)),
        glm::lookAt(glm::vec3(0,0,0), glm::vec3( 0, 0, 1), glm::vec3(0,-1, 0)),
        glm::lookAt(glm::vec3(0,0,0), glm::vec3( 0, 0,-1), glm::vec3(0,-1, 0))
    };

    // Prepare FBO
    glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo);
    glBindRenderbuffer(GL_RENDERBUFFER, capture_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, cubemap_size, cubemap_size);

    // Shader uniforms
    capture_shader->activate_shader();
    capture_shader->set_int("equirectangularMap", 0);
    capture_shader->set_mat4("projection", capture_proj);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdr_texture);

    // Render to each cubemap face
    glViewport(0, 0, cubemap_size, cubemap_size);
    for (unsigned int i = 0; i < 6; ++i) {
        capture_shader->set_mat4("view", capture_views[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cube->draw(render::VertexLayouts::PositionOnly);
    }

    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);


    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return cubemap;
}

std::shared_ptr<SkyBox> init_hdri_skybox(const std::shared_ptr<SkyBoxManager> skybox_manager, const std::string name, const std::string& hdr_path) {
    auto capture_shader = std::make_shared<render::Shader>("shaders/hdr_to_cubemap.vert", "shaders/hdr_to_cubemap.frag");
    auto render_shader = std::make_shared<render::Shader>("shaders/skybox.vert", "shaders/skybox.frag");

    // 1. Load HDR texture
    GLuint hdr_texture = render::load_hdr_texture(hdr_path);
    if (hdr_texture == 0) {
        LOG_ERROR("Failed to load HDR texture at path: " + hdr_path);
        return nullptr;
    }

    // 2. Setup framebuffer and renderbuffer for capturing cubemap
    GLuint capture_fbo, capture_rbo;
    glGenFramebuffers(1, &capture_fbo);
    glGenRenderbuffers(1, &capture_rbo);

    // 3. Convert HDR texture to cubemap
    GLuint cubemap = equirectangular_to_cubemap(hdr_texture, capture_shader, capture_fbo, capture_rbo);

    // 4. Clean up FBOs
    glDeleteFramebuffers(1, &capture_fbo);
    glDeleteRenderbuffers(1, &capture_rbo);
    glDeleteTextures(1, &hdr_texture); // We don’t need the 2D HDR texture anymore

    // 5. Create skybox material
    auto skybox_material = std::make_shared<render::Material>(render_shader, std::make_shared<render::Texture>(cubemap, GL_TEXTURE_CUBE_MAP, GL_TEXTURE0));
    skybox_material->sampler_name = "skybox";
    skybox_material->bind_uniforms = render::UniformPresets::skybox_bind;
    skybox_material->set_vertex_layout(render::VertexLayouts::PositionOnly);

    // 6. Setup cube mesh
    auto skybox_mesh = std::make_shared<render::Mesh>();
    skybox_mesh->init_positions_only(skybox_vertices, sizeof(skybox_vertices));
    skybox_mesh->set_vertex_cnt(36);
    skybox_mesh->set_draw_mode(MeshDrawMode::Arrays);

    // 7. Create SkyBox and return shared_ptr
    auto skybox = std::make_shared<SkyBox>(skybox_mesh, skybox_material);

    skybox_manager->register_factory(name, [skybox_mesh, skybox_material]() {
        return std::make_shared<SkyBox>(skybox_mesh, skybox_material);
    });

    return skybox;
}

float skybox_vertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

}
