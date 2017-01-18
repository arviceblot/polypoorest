#pragma once

#include <glm/vec3.hpp>

#include "Basis.h"
#include "Light.h"

class AreaLight : public Light
{
public:
    AreaLight(
        const std::string &type,
        const glm::vec3 &position = glm::vec3(0.0f),
        const glm::vec3 &intensity = glm::vec3(0.0f),
        const glm::vec3 &normal = glm::vec3(0.0f, -1.0f, 0.0f),
        const float &width = 1.0f,
        const float &length = 1.0f
    );
    virtual ~AreaLight();

    virtual const glm::vec3 & getPosition() const;

    inline const Basis & getBasis() const
    {
        return basis;
    }
    inline const float & getWidth() const
    {
        return width;
    }
    inline const float & getLength() const
    {
        return length;
    }

private:
    Basis basis;
    float width;
    float length;
};
