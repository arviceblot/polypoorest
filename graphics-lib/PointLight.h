#pragma once

#include <glm/vec3.hpp>
#include "Light.h"

/**
 * Currently no real implementation, is this really needed?
 */
class PointLight : public Light
{
public:
    PointLight(
        const std::string &type,
        const glm::vec3 &position = glm::vec3(0.0f),
        const glm::vec3 &intensity = glm::vec3(0.0f)
    ) : Light(type, position, intensity) {}
    virtual ~PointLight() {}
};
