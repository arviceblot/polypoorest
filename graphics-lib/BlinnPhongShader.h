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
                     const float &roughness = 0.0f)
        : Shader(name, type, diffuse),
          specular(specular),
          phongExponent(phongExponent),
          mirrorCoefficient(mirrorCoefficient),
          roughness(roughness) {}
    virtual ~BlinnPhongShader() {}

    inline const glm::vec3 & getSpecular() const
    {
        return specular;
    }
    inline const float & getPhongExponent() const
    {
        return phongExponent;
    }
    inline const float & getMirrorCoefficient() const
    {
        return mirrorCoefficient;
    }

protected:
    virtual void perLightOperation(Scene *scene, Light *light, RaycastHit &hit, int depth, glm::vec3 &color);
    virtual void postLightingOperation(Scene *scene, RaycastHit & hit, int depth, glm::vec3 &color);

    glm::vec3 specular;
    float phongExponent;
    float mirrorCoefficient;
    float roughness;
};
