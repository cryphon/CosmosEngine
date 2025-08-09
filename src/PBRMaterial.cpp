#include "PBRMaterial.hpp"
#include "Logger.hpp"


void PBRMaterial::bind_texture(const std::shared_ptr<Shader>& shader,
                               const std::shared_ptr<Texture>& texture,
                               bool use_texture,
                               int texture_unit,
                               const std::string& name) {
    std::string use_name = "use" + name;
    std::string uniform_name = "u" + name;

    if (use_texture && texture) {
        texture->bind(GL_TEXTURE0 + texture_unit);
        shader->set_bool(use_name, true);
        shader->set_int(uniform_name, texture_unit);
    } else {
        shader->set_bool(use_name, false);
    }
}


void PBRMaterial::bind() {
    shader->activate_shader();

    shader->set_vec3("uAlbedo", albedo);
    shader->set_float("uRoughness", roughness);
    shader->set_float("uMetallic", metallic);
    shader->set_float("uAlpha", alpha);
    shader->set_float("uReflectivity", reflectivity);
    shader->set_float("uTiling", tiling);
    shader->set_float("uDisplacementStrength", displacement);

    shader->set_bool("useAlbedoMap", useAlbedoMap);
    shader->set_bool("useRoughnessMap", useRoughnessMap);
    shader->set_bool("useNormalMap", useNormalMap);
    shader->set_bool("useMetallicMap", useMetallicMap);
    shader->set_bool("useAOMap", useAoMap);
    shader->set_bool("useDisplacementMap", useDisplacementMap);







    bind_texture(shader, albedoMap, useAlbedoMap, 0, "AlbedoMap");
    bind_texture(shader, normalMap, useNormalMap, 1, "NormalMap");
    bind_texture(shader, roughnessMap, useRoughnessMap, 2, "RoughnessMap");
    bind_texture(shader, metallicMap, useMetallicMap, 3, "MetallicMap");
    bind_texture(shader, aoMap, useAoMap, 4, "AoMap");
    bind_texture(shader, displacementMap, useDisplacementMap, 5, "DisplacementMap");
}

