#pragma once

#include "Shape.h"

class BVHNode : public Shape
{
public:
    BVHNode(std::vector<Shape *> shapes, int axis);
    virtual ~BVHNode();

    virtual bool isClosestHit(
        const Ray &ray,
        const float &tMin,
        float &tMax,
        RaycastHit &hit
    );

    inline const Shape * getLeftChild() const
    {
        return leftChild;
    }
    inline const Shape * getRightChild() const
    {
        return rightChild;
    }

protected:
    Shape *leftChild;
    Shape *rightChild;
    int axis;

private:
    std::vector<Shape *> mergeSort(std::vector<Shape *> &vec);
    std::vector<Shape *> merge(const std::vector<Shape *> &left, const std::vector<Shape *> &right);
};
