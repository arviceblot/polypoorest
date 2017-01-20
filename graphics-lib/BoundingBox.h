#pragma once

#include <glm/vec3.hpp>
#include "Mathf.h"
#include "Shape.h"

class BoundingBox : public Shape
{
public:
    BoundingBox(
        const glm::vec3 &minPoint = glm::vec3(Mathf::Max, Mathf::Max, Mathf::Max),
        const glm::vec3 &maxPoint = glm::vec3(Mathf::Min, Mathf::Min, Mathf::Min));
    virtual ~BoundingBox();

    virtual bool isClosestHit(
        const Ray &ray,
        const float &tMin,
        float &tMax,
        RaycastHit &hit
    );

    void addVertex(const glm::vec3 &vertex);
    void merge(const BoundingBox *other);

    const glm::vec3 & getMinPoint() const;
    const glm::vec3 & getMaxPoint() const;

protected:
    glm::vec3 minPoint;
    glm::vec3 maxPoint;
};

inline BoundingBox::BoundingBox(const glm::vec3 &minPoint, const glm::vec3 &maxPoint)
    : Shape("", "", NULL), minPoint(minPoint), maxPoint(maxPoint)
{
    bbox = this;
}

inline BoundingBox::~BoundingBox() {}

inline const glm::vec3 & BoundingBox::getMinPoint() const
{
    return minPoint;
}

inline const glm::vec3 & BoundingBox::getMaxPoint() const
{
    return maxPoint;
}
