#pragma once

#include <glm/vec3.hpp>

/**
 * TODO: This could probably be a struct since it's really just holding data.
 */
class Ray
{
public:
    Ray(const glm::vec3 &origin = glm::vec3(0.0f), const glm::vec3 &direction = glm::vec3(0.0f, 0.0f, -1.0f))
        : origin(origin), direction(direction) {}
    Ray(const Ray &otherRay)
        : origin(otherRay.origin), direction(otherRay.direction) {}
    virtual ~Ray() {}

    inline const glm::vec3 & getOrigin() const
    {
        return origin;
    }
    inline const glm::vec3 & getDirection() const
    {
        return direction;
    }

    inline void setOrigin(const glm::vec3 &newOrigin)
    {
        origin = newOrigin;
    }
    inline void setDirection(const glm::vec3 &newDirection)
    {
        direction = newDirection;
    }

private:
    glm::vec3 origin;
    glm::vec3 direction;
};
