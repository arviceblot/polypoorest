#pragma once

#include <glm/vec3.hpp>

struct Ray
{
    Ray(const glm::vec3 &origin = glm::vec3(0.0f), const glm::vec3 &direction = glm::vec3(0.0f, 0.0f, -1.0f));
    Ray(const Ray &otherRay);
    ~Ray();

    glm::vec3 origin;
    glm::vec3 direction;
};

inline Ray::Ray(const glm::vec3 &origin, const glm::vec3 &direction)
    : origin(origin), direction(direction) {}
inline Ray::Ray(const Ray &otherRay)
    : origin(otherRay.origin), direction(otherRay.direction) {}
inline Ray::~Ray() {}
