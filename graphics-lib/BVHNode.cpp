#include "BVHNode.h"
#include <cstdlib>
#include "BoundingBox.h"
#include "RaycastHit.h"

BVHNode::BVHNode(
    std::vector<Shape *> shapes,
    int axis
) : Shape("", "", NULL), axis(axis)
{
    bbox = new BoundingBox();

    if (shapes.size() == 1)
    {
        leftChild = shapes[0];
        rightChild = NULL;
        bbox->merge(leftChild->getBoundingBox());
    }
    else if (shapes.size() == 2)
    {
        leftChild = shapes[0];
        rightChild = shapes[1];
        bbox->merge(leftChild->getBoundingBox());
        bbox->merge(rightChild->getBoundingBox());
    }
    else
    {
        // sort and split
        shapes = mergeSort(shapes);

        std::vector<Shape*>::iterator middle = shapes.begin() + (shapes.size() / 2);

        std::vector<Shape *> left(shapes.begin(), middle);
        std::vector<Shape *> right(middle, shapes.end());

        leftChild = new BVHNode(left, (axis + 1) % 3);
        rightChild = new BVHNode(right, (axis + 1) % 3);

        bbox->merge(leftChild->getBoundingBox());
        bbox->merge(rightChild->getBoundingBox());
    }
}

BVHNode::~BVHNode()
{
    delete bbox;
    if (leftChild != NULL)
    {
        delete leftChild;
    }
    if (rightChild != NULL)
    {
        delete rightChild;
    }
}

bool BVHNode::isClosestHit(
    const Ray &ray,
    const float &tMin,
    float &tMax,
    RaycastHit &hit)
{
    if (bbox->isClosestHit(ray, tMin, tMax, hit))
    {
        RaycastHit lHit = hit;
        RaycastHit rHit = hit;
        float ltMax = tMax;
        float rtMax = tMax;

        bool leftHit = (leftChild != NULL) && leftChild->isClosestHit(ray, tMin, ltMax, lHit);
        bool rightHit = (rightChild != NULL) && rightChild->isClosestHit(ray, tMin, rtMax, rHit);

        if (leftHit && rightHit)
        {
            // both sides were hit, return the closest of left and right
            if (ltMax < rtMax)
            {
                // left hit "first"
                tMax = ltMax;
                hit = lHit;
            }
            else
            {
                // right hit "first"
                tMax = rtMax;
                hit = rHit;
            }
            return true;
        }
        else if (leftHit)
        {
            // only left side was hit
            tMax = ltMax;
            hit = lHit;
            return true;
        }
        else if (rightHit)
        {
            // only right side was hit
            tMax = rtMax;
            hit = rHit;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

/**
 * https://en.wikibooks.org/wiki/Algorithm_Implementation/Sorting/Merge_sort#C.2B.2B
 */
std::vector<Shape *> BVHNode::mergeSort(std::vector<Shape *> &vec)
{
    if (vec.size() == 1)
    {
        return vec;
    }

    std::vector<Shape *>::iterator middle = vec.begin() + (vec.size() / 2);

    std::vector<Shape *> left(vec.begin(), middle);
    std::vector<Shape *> right(middle, vec.end());

    left = mergeSort(left);
    right = mergeSort(right);

    return merge(left, right);
}

/**
 * https://en.wikibooks.org/wiki/Algorithm_Implementation/Sorting/Merge_sort#C.2B.2B
 */
std::vector<Shape *> BVHNode::merge(const std::vector<Shape *> &left, const std::vector<Shape *> &right)
{
    std::vector<Shape *> result;
    unsigned lit = 0;
    unsigned rit = 0;

    while (lit < left.size() && rit < right.size())
    {
        // sort by object center along axis
        float lcenter = ((left[lit]->getBoundingBox()->getMinPoint() + left[lit]->getBoundingBox()->getMaxPoint()) / 2.0f)[axis];
        float rcenter = ((right[rit]->getBoundingBox()->getMinPoint() + right[rit]->getBoundingBox()->getMaxPoint()) / 2.0f)[axis];

        if (lcenter < rcenter)
        {
            // left < right
            result.push_back(left[lit]);
            lit++;
        }
        else
        {
            result.push_back(right[rit]);
            rit++;
        }
    }

    while (lit < left.size())
    {
        result.push_back(left[lit]);
        lit++;
    }
    while (rit < right.size())
    {
        result.push_back(right[rit]);
        rit++;
    }

    return result;
}
