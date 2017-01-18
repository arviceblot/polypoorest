#pragma once

#include <glm/glm.hpp>
#include "Shape.h"

class InstancedObject : public Shape
{
public:
    InstancedObject(
        const std::string &name,
        const std::string &type,
        Shape *shape,
        const glm::mat4 &transform
    );
    virtual ~InstancedObject();

    virtual bool isClosestHit(
        const Ray& ray,
        const float& tMin,
        float& tMax,
        RaycastHit& hit
    );

    inline const Shape * getBase() const
    {
        return base;
    }
    inline const glm::mat4 & getTransform() const
    {
        return transform;
    }

protected:
    Shape *base;
    glm::mat4 transform;
    glm::mat4 invTransform;
};
