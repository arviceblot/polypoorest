#include "InstancedObject.h"
#include <cstdlib>
#include "RaycastHit.h"

InstancedObject::InstancedObject(const std::string &name, const std::string &type, Shape *shape, const glm::mat4 &transform)
    : Shape(name, type, NULL), base(shape), transform(transform)
{
    invTransform = glm::inverse(transform);
}

InstancedObject::~InstancedObject()
{
}

bool InstancedObject::isClosestHit(const Ray &ray, const float &tMin, float &tMax, RaycastHit &hit)
{
    // see slide_Instancing.pdf[51] for algorithm
    // Ray r' = M^-1 r.origin + tM^-1 r.direction
    //
    // if (baseObjectPtr→intersect(r', tmin, tmax, rec))
    //     rec.n = (M^-1)^T rec.n
    //     return true;
    // else
    //     return false;
    Ray transRay;
    transRay.setOrigin(glm::vec3((invTransform * glm::vec4(ray.getOrigin(), 1.0f))));
    transRay.setDirection(glm::vec3((invTransform * glm::vec4(ray.getDirection(), 1.0f))));

    if (base->isClosestHit(transRay, tMin, tMax, hit))
    {
        hit.normal = glm::vec3(glm::transpose(invTransform) * glm::vec4(hit.normal, 1.0f));
        return true;
    }

    return false;
}
