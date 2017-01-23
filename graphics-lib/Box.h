#pragma once

#include "Shape.h"

class Box : public Shape
{
public:
    Box(const std::string &name,
        const std::string &type,
        std::shared_ptr<Shader> shaderRef,
        const glm::vec3 &minPoint = glm::vec3(0.0f, 0.0f, 0.0f),
        const glm::vec3 &maxPoint = glm::vec3(0.0f, 0.0f, 0.0f));

    virtual bool isClosestHit(const Ray &ray, const float &tMin, float &tMax, RaycastHit &hit);
};
