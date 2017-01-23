#include "BoundingBox.h"

bool BoundingBox::isClosestHit(const Ray &ray, const float &tMin, float &tMax, RaycastHit &hit)
{
    glm::vec3 tmin;
    glm::vec3 tmax;
    glm::vec3 rayDir = ray.direction;
    glm::vec3 invDir(
        1.0f / rayDir[0],
        1.0f / rayDir[1],
        1.0f / rayDir[2]
    );
    for (int i = 0; i < 3; i++)
    {
        if (rayDir[i] < 0.0f)
        {
            tmin[i] = (maxPoint[i] - ray.origin[i]) * invDir[i];
            tmax[i] = (minPoint[i] - ray.origin[i]) * invDir[i];
        }
        else
        {
            tmin[i] = (minPoint[i] - ray.origin[i]) * invDir[i];
            tmax[i] = (maxPoint[i] - ray.origin[i]) * invDir[i];
        }
    }

    if (tmin[0] > tmax[1] || tmin[1] > tmax[0])
    {
        return false;
    }
    if (tmin[1] > tmin[0])
    {
        tmin[0] = tmin[1];
    }
    if (tmax[1] < tmax[0])
    {
        tmax[0] = tmax[1];
    }
    if (tmin[0] > tmax[2] || tmin[2] > tmax[0])
    {
        return false;
    }
    if (tmin[2] > tmin[0])
    {
        tmin[0] = tmin[2];
    }
    if (tmax[2] < tmax[0])
    {
        tmax[0] = tmax[2];
    }
    return (tmin[0] < tMax && tmax[0] > tMin);
}

void BoundingBox::addVertex(const glm::vec3 &vertex)
{
    for (int i = 0; i < 3; i++)
    {
        if (vertex[i] < minPoint[i])
        {
            minPoint[i] = vertex[i];
        }
        if (vertex[i] > maxPoint[i])
        {
            maxPoint[i] = vertex[i];
        }
    }
}

void BoundingBox::merge(const std::shared_ptr<BoundingBox> other)
{
    addVertex(other->minPoint);
    addVertex(other->maxPoint);
}
