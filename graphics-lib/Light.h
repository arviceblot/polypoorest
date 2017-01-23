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
    );

    virtual const glm::vec3 getPosition() const;
    virtual const glm::vec3 & getIntensity() const;

protected:
    glm::vec3 position;
    glm::vec3 intensity;

private:
    Light();
};

inline Light::Light(const std::string& type, const glm::vec3 &position, const glm::vec3 &intensity)
    : SceneElement("", type, SceneElement::Type::LIGHT),
      position(position),
      intensity(intensity) {}

inline const glm::vec3 Light::getPosition() const
{
    return position;
}

inline const glm::vec3 & Light::getIntensity() const
{
    return intensity;
}
