#pragma once

#include <glm/vec3.hpp>

#include "Basis.h"
#include "Light.h"

class AreaLight : public Light
{
public:
    AreaLight(const std::string &type,
              const glm::vec3 &position = glm::vec3(0.0f),
              const glm::vec3 &intensity = glm::vec3(0.0f),
              const glm::vec3 &normal = glm::vec3(0.0f, -1.0f, 0.0f),
              const float &width = 1.0f,
              const float &length = 1.0f);

    virtual const glm::vec3 getPosition() const;

    const Basis & getBasis() const;
    const float & getWidth() const;
    const float & getLength() const;

private:
    Basis basis;
    float width;
    float length;
};

inline AreaLight::AreaLight(const std::string &type,
                            const glm::vec3 &position,
                            const glm::vec3 &intensity,
                            const glm::vec3 &normal,
                            const float &width,
                            const float &length)
    : Light(type, position, intensity),
      basis(normal, glm::vec3(1.0f, 0.0f, 0.0f)),
      width(width),
      length(length)
{
    basis.setU(basis.getU() * width);
    basis.setV(basis.getV() * length);
}

inline const Basis & AreaLight::getBasis() const
{
    return basis;
}

inline const float & AreaLight::getWidth() const
{
    return width;
}

inline const float & AreaLight::getLength() const
{
    return length;
}
