#pragma once

#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include "Ray.h"
#include "SceneElement.h"
#include "Shader.h"

class RaycastHit;
class BoundingBox;
class Triangle;

class Shape : public SceneElement
{
public:
    Shape(
        const std::string &name,
        const std::string &type,
        Shader *shaderRef
    ) : SceneElement(name, type, SceneElement::Type::SHAPE), shaderRef(shaderRef) {}
    virtual ~Shape() = 0;

    virtual bool isClosestHit(
        const Ray &ray,
        const float &tMin,
        float &tMax,
        RaycastHit &hit
    ) = 0;

    virtual std::vector<Triangle *> getTriangles()
    {
        return triangles;
    }
    virtual std::vector<glm::vec3> getVerticies()
    {
        return verticies;
    }
    virtual const Shader * getShader()
    {
        return shaderRef;
    }
    virtual const BoundingBox * getBoundingBox()
    {
        return bbox;
    }

protected:
    Shader *shaderRef;
    BoundingBox *bbox;
    std::vector<Triangle *> triangles;
    std::vector<glm::vec3> verticies;

private:
    Shape();
};
