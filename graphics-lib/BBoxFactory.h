#pragma once

#include <memory>

#include "BoundingBox.h"

class BBoxFactory
{
public:
    static std::shared_ptr<BoundingBox> Create(
        const glm::vec3 &minPoint = glm::vec3(Mathf::Max, Mathf::Max, Mathf::Max),
        const glm::vec3 &maxPoint = glm::vec3(Mathf::Min, Mathf::Min, Mathf::Min));
};

inline std::shared_ptr<BoundingBox> BBoxFactory::Create(const glm::vec3 &minPoint, const glm::vec3 &maxPoint)
{
    auto bbox = std::make_shared<BoundingBox>(minPoint, maxPoint);
    bbox->finalize();
    return bbox;
}
