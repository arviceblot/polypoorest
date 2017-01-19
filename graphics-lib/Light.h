#pragma once

#include <string>
#include <glm/vec3.hpp>
#include "SceneElement.h"

/**
 * Abstract class for representing lights.
 */
class Light : public SceneElement
{
public:
    Light(
        const std::string& type,
        const glm::vec3 &position = glm::vec3(0.0f),
        const glm::vec3 &intensity = glm::vec3(0.0f)
    ) : SceneElement("", type, SceneElement::Type::LIGHT), position(position),
        intensity(intensity) {}
    virtual ~Light() = 0;

    virtual const glm::vec3 getPosition() const
    {
        return position;
    }
    virtual const glm::vec3 & getIntensity() const
    {
        return intensity;
    }

protected:
    glm::vec3 position;
    glm::vec3 intensity;

private:
    Light();
};
