#pragma once

#include <glm/vec3.hpp>
#include "Shape.h"

class Triangle : public Shape, public std::enable_shared_from_this<Triangle>
{
public:
    Triangle(const std::string &name,
             const std::string &type,
             std::shared_ptr<Shader> shaderRef,
             const glm::vec3 &a = glm::vec3(0.0f),
             const glm::vec3 &b = glm::vec3(0.0f),
             const glm::vec3 &c = glm::vec3(0.0f));

    virtual bool isClosestHit(const Ray& ray, const float& tMin, float& tMax, RaycastHit& hit);

    void finalize();

    const glm::vec3 & getSurfaceNormal() const;

protected:
    glm::vec3 A;
    glm::vec3 B;
    glm::vec3 C;
    glm::vec3 surfaceNormal;
};

inline void Triangle::finalize()
{
    // add this triangle to the vector of triangles
    triangles.push_back(shared_from_this());
}

inline const glm::vec3 & Triangle::getSurfaceNormal() const
{
    return surfaceNormal;
}
