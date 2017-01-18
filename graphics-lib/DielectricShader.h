#pragma once

#include "Shader.h"

class DielectricShader : public Shader
{
public:
    DielectricShader(const std::string &name,
                     const std::string &type,
                     const float &refractiveIndex,
                     const glm::vec3 & attenuationCoefficient = glm::vec3(0.0f))
        : Shader(name, type),
          refractiveIndex(refractiveIndex),
          attenuationCoefficient(attenuationCoefficient) {}
    virtual ~DielectricShader() {}

    inline const float & getRefractiveIndex() const
    {
        return refractiveIndex;
    }
    inline const glm::vec3 & getAttenuationCoefficient() const
    {
        return attenuationCoefficient;
    }

protected:
    float refractiveIndex;
    glm::vec3 attenuationCoefficient;
};
