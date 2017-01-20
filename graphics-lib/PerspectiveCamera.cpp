#include "PerspectiveCamera.h"
#include "Ray.h"

void PerspectiveCamera::computeRay(Ray &ray, float i, float j)
{
    // assume symmetric camera
    float l = imagePlaneWidth * -0.5f;
    float r = -l;
    float t = (imagePlaneWidth / aspectRatio) * -0.5f;
    float b = -t;

    float u = l + ((r - l) * (i + 0.5f)) / size.x;
    float v = b + ((t - b) * (j + 0.5f)) / size.y;

    ray.direction = -focalLength * basis.getW()
                    + u * basis.getU()
                    + v * basis.getV();
    ray.origin = position;
}
