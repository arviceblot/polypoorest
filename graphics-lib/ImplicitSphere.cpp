#include "ImplicitSphere.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include "BoundingBox.h"
#include "RaycastHit.h"
#include "Triangle.h"

ImplicitSphere::ImplicitSphere(
    const std::string &name,
    const std::string &type,
    Shader *shaderRef,
    const glm::vec3 &center,
    float radius
) : Shape(name, type, shaderRef), center(center), radius(radius)
{
    glm::vec3 minPoint = center - glm::vec3(radius);
    glm::vec3 maxPoint = center + glm::vec3(radius);

    // create the bounding box
    bbox = new BoundingBox(minPoint, maxPoint);
}

ImplicitSphere::~ImplicitSphere()
{
    // delete all the triangles
    if (!triangles.empty())
    {
        for (std::vector<Triangle*>::iterator it = triangles.begin(); it != triangles.end();)
        {
            delete (*it);
            it = triangles.erase(it);
        }
    }

    verticies.clear();
    delete bbox;
}

bool ImplicitSphere::isClosestHit(const Ray &ray, const float &tMin, float &tMax, RaycastHit &hit)
{
    // check the bounding box first
    if (!bbox->isClosestHit(ray, tMin, tMax, hit))
    {
        return false;
    }

    float a = glm::dot(ray.direction, ray.direction);
    float b = glm::dot(ray.direction * glm::vec3(2.0f), ray.origin - center);
    float c = glm::dot((ray.origin - center), (ray.origin - center)) - std::pow(radius, 2);
    float descriminant = std::pow(b, 2) - (4 * a * c);

    if (descriminant < 0)
    {
        return false;
    }
    else
    {
        float denominator = 1.0f / (2 * a);
        float t1 = (-b + std::sqrt(descriminant)) * denominator;
        float t2 = (-b - std::sqrt(descriminant)) * denominator;
        float t = std::min(t1, t2);

        if (t > tMin && t < tMax)
        {
            tMax = t;
            glm::vec3 p = ray.origin + (glm::vec3(t) * ray.direction); // point of intersection
            glm::vec3 surfaceNormal = p - center;
            surfaceNormal = glm::normalize(surfaceNormal);

            // set the hit structure properties
            hit.normal = surfaceNormal;
            hit.point = p;
            hit.shape = this;
            hit.shader = shaderRef;
            hit.sourceRayDir = ray.direction;

            return true;
        }
    }
    return false;
}
