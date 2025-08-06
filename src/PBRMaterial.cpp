#include "PBRMaterial.hpp"
#include "Logger.hpp"


void PBRMaterial::bind() {
    shader->activate_shader();

    // Send scalar uniforms
    shader->set_vec3("uAlbedo", albedo);
    shader->set_float("uRoughness", roughness);
    shader->set_float("uMetallic", metallic);
    shader->set_float("uAlpha", alpha);
    shader->set_float("uReflectivity", reflectivity);

    // Texture 0: Albedo
    if (albedoMap) {
        albedoMap->bind(GL_TEXTURE0);
        shader->set_bool("useAlbedoMap", true);
        shader->set_int("uAlbedoMap", 0);
    } else {
        shader->set_bool("useAlbedoMap", false);
    }

    // Texture 1: Normal
    if (normalMap) {
        normalMap->bind(GL_TEXTURE1);
        shader->set_bool("useNormalMap", true);
        shader->set_int("uNormalMap", 1);
    } else {
        shader->set_bool("useNormalMap", false);
    }

    // Texture 2: Roughness
    if (roughnessMap) {
        roughnessMap->bind(GL_TEXTURE2);
        shader->set_bool("useRoughnessMap", true);
        shader->set_int("uRoughnessMap", 2);
    } else {
        shader->set_bool("useRoughnessMap", false);
    }

    if(metallicMap) {
        metallicMap->bind(GL_TEXTURE3);
        shader->set_bool("useMetallicMap", true);
        shader->set_int("uMetallicMap", 3);
    } else {
        shader->set_bool("useMetallicMap", false);
    }

    if(aoMap) {
        aoMap->bind(GL_TEXTURE4);
        shader->set_bool("useAOMap", true);
        shader->set_int("uAOMap", 4);
    } else {
        shader->set_bool("useAOMap", false);
    }
    // ... repeat for metallicMap, aoMap as needed
}

