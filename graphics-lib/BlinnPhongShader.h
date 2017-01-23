#pragma once

#include <glm/vec3.hpp>
#include "Shader.h"

class BlinnPhongShader : public Shader
{
public:
    BlinnPhongShader(const std::string &name,
                     const std::string &type,
                     const glm::vec3 &diffuse = glm::vec3(0.0f),
                     const glm::vec3 &specular = glm::vec3(0.0f),
                     const float &phongExponent = 0.0f,
                     const float &mirrorCoefficient = 0.0f,
                     const float &roughness = 0.0f);

    const glm::vec3 & getSpecular() const;
    const float & getPhongExponent() const;
    const float & getMirrorCoefficient() const;

protected:
    virtual void perLightOperation(Scene *scene, std::shared_ptr<Light> light, RaycastHit &hit, int depth, glm::vec3 &color);
    virtual void postLightingOperation(Scene *scene, RaycastHit & hit, int depth, glm::vec3 &color);

    glm::vec3 specular;
    float phongExponent;
    float mirrorCoefficient;
    float roughness;
};

inline BlinnPhongShader::BlinnPhongShader(const std::string &name,
        const std::string &type,
        const glm::vec3 &diffuse,
        const glm::vec3 &specular,
        const float &phongExponent,
        const float &mirrorCoefficient,
        const float &roughness)
    : Shader(name, type, diffuse),
      specular(specular),
      phongExponent(phongExponent),
      mirrorCoefficient(mirrorCoefficient),
      roughness(roughness) {}

inline const glm::vec3 & BlinnPhongShader::getSpecular() const
{
    return specular;
}

inline const float & BlinnPhongShader::getPhongExponent() const
{
    return phongExponent;
}

inline const float & BlinnPhongShader::getMirrorCoefficient() const
{
    return mirrorCoefficient;
}
