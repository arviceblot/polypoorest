#pragma once

#include "Shape.h"

class ImplicitSphere : public Shape
{
public:
    ImplicitSphere(
        const std::string &name,
        const std::string &type,
        Shader *shaderRef,
        const glm::vec3 &center = glm::vec3(0.0f),
        float radius = 1.0f
    );
    virtual ~ImplicitSphere();

    virtual bool isClosestHit(
        const Ray &ray,
        const float &tMin,
        float &tMax,
        RaycastHit &hit
    );

private:
    glm::vec3 center;
    float radius;
};
