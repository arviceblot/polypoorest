#pragma once

#include "Shape.h"

class BVHNode : public Shape
{
public:
    BVHNode(std::vector<std::shared_ptr<Shape>> shapes, int axis);

    virtual bool isClosestHit(const Ray &ray, const float &tMin, float &tMax, RaycastHit &hit);

    inline const std::shared_ptr<Shape> getLeftChild() const;

    inline const std::shared_ptr<Shape> getRightChild() const;

protected:
    std::shared_ptr<Shape> leftChild;
    std::shared_ptr<Shape> rightChild;
    int axis;

private:
    std::vector<std::shared_ptr<Shape>> mergeSort(std::vector<std::shared_ptr<Shape>> &vec);

    std::vector<std::shared_ptr<Shape>> merge(const std::vector<std::shared_ptr<Shape>> &left, const std::vector<std::shared_ptr<Shape>> &right);
};

inline const std::shared_ptr<Shape> BVHNode::getLeftChild() const
{
    return leftChild;
}

inline const std::shared_ptr<Shape> BVHNode::getRightChild() const
{
    return rightChild;
}
