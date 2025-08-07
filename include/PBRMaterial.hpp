#pragma once
#include "Material.hpp"

class PBRMaterial : public Material {
private:
    typedef Material super;
public:
    PBRMaterial(const std::shared_ptr<Shader> shader) 
    : super(shader) {}
    void bind() override;


    glm::vec3 albedo = glm::vec3(0.0f);
    float roughness = 0.5f;
    float metallic = 0.0f;
    float alpha = 1.0f;
    float reflectivity = 0.0f;
    float tiling = 1.0f;

    std::shared_ptr<Texture> albedoMap = nullptr;
    std::shared_ptr<Texture> normalMap = nullptr;
    std::shared_ptr<Texture> roughnessMap = nullptr;
    std::shared_ptr<Texture> metallicMap = nullptr;
    std::shared_ptr<Texture> aoMap = nullptr;
    std::shared_ptr<Texture> displacementMap = nullptr;
};
