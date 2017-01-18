#pragma once

#include <limits>
#include <glm/vec3.hpp>
#include "Shape.h"

class BoundingBox : public Shape
{
public:
    BoundingBox(
        const glm::vec3 &minPoint = glm::vec3(
                                        std::numeric_limits<float>::max(),
                                        std::numeric_limits<float>::max(),
                                        std::numeric_limits<float>::max()
                                    ),
        const glm::vec3 &maxPoint = glm::vec3(
                                        std::numeric_limits<float>::min(),
                                        std::numeric_limits<float>::min(),
                                        std::numeric_limits<float>::min()
                                    )
    );
    virtual ~BoundingBox();

    virtual bool isClosestHit(
        const Ray &ray,
        const float &tMin,
        float &tMax,
        RaycastHit &hit
    );

    void addVertex(const glm::vec3 &vertex);
    void merge(const BoundingBox *other);

    inline const glm::vec3 & getMinPoint() const
    {
        return minPoint;
    }
    inline const glm::vec3 & getMaxPoint() const
    {
        return maxPoint;
    }

protected:
    glm::vec3 minPoint;
    glm::vec3 maxPoint;
};
