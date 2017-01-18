#pragma once

#include <glm/glm.hpp>

#include "Shader.h"

class LambertianShader : public Shader
{
public:
    LambertianShader(const std::string& name,
                     const std::string& type,
                     const glm::vec3 &diffuse = glm::vec3(0.0f),
                     const float& mirrorCoefficient = 0.0f)
        : Shader(name, type, diffuse), mirrorCoefficient(mirrorCoefficient) {}
    virtual ~LambertianShader() {}

protected:
    virtual void perLightOperation(Scene *scene, Light *light, RaycastHit &hit, int depth, glm::vec3 &color);
    virtual void postLightingOperation(Scene *scene, RaycastHit & hit, int depth, glm::vec3 &color);

    float mirrorCoefficient;
};
