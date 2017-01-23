#pragma once

#include <memory>
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
    Shape(const std::string &name, const std::string &type, std::shared_ptr<Shader> shaderRef);

    virtual bool isClosestHit(const Ray &ray, const float &tMin, float &tMax, RaycastHit &hit) = 0;

    virtual std::vector<std::shared_ptr<Triangle>> getTriangles();

    virtual std::vector<glm::vec3> getVerticies();

    virtual const std::shared_ptr<Shader> getShader();

    virtual const std::shared_ptr<BoundingBox> getBoundingBox();

protected:
    std::shared_ptr<Shader> shaderRef;
    std::shared_ptr<BoundingBox> bbox;
    std::vector<std::shared_ptr<Triangle>> triangles;
    std::vector<glm::vec3> verticies;

private:
    Shape();
};

inline Shape::Shape(const std::string &name, const std::string &type, std::shared_ptr<Shader> shaderRef)
    : SceneElement(name, type, SceneElement::Type::SHAPE), shaderRef(shaderRef) {}

inline std::vector<std::shared_ptr<Triangle>> Shape::getTriangles()
{
    return triangles;
}

inline std::vector<glm::vec3> Shape::getVerticies()
{
    return verticies;
}

inline const std::shared_ptr<Shader> Shape::getShader()
{
    return shaderRef;
}

inline const std::shared_ptr<BoundingBox> Shape::getBoundingBox()
{
    return bbox;
}
