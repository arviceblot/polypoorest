#pragma once

#include <glm/vec3.hpp>
#include "Shape.h"

class Triangle : public Shape
{
public:
    Triangle(
        const std::string &name,
        const std::string &type,
        Shader *shaderRef,
        const glm::vec3 &a = glm::vec3(0.0f),
        const glm::vec3 &b = glm::vec3(0.0f),
        const glm::vec3 &c = glm::vec3(0.0f)
    );
    virtual ~Triangle();

    virtual bool isClosestHit(
        const Ray& ray,
        const float& tMin,
        float& tMax,
        RaycastHit& hit
    );

    inline const glm::vec3 & getSurfaceNormal() const { return surfaceNormal; }

protected:
    glm::vec3 A;
    glm::vec3 B;
    glm::vec3 C;
    glm::vec3 surfaceNormal;
};
