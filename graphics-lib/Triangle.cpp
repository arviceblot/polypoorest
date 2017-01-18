#include "Triangle.h"
#include <glm/glm.hpp>
#include "BoundingBox.h"
#include "RaycastHit.h"

Triangle::Triangle(
    const std::string &name, const std::string &type,
    Shader *shaderRef, const glm::vec3 &a,
    const glm::vec3 &b, const glm::vec3 &c)
    : Shape(name, type, shaderRef), A(a), B(b), C(c)
{
    // add this triangle to the vector of triangles
    triangles.push_back(this);

    // build the bounding box
    bbox = new BoundingBox();
    bbox->addVertex(A);
    bbox->addVertex(B);
    bbox->addVertex(C);

    // add the verticies
    verticies.push_back(A);
    verticies.push_back(B);
    verticies.push_back(C);

    surfaceNormal = glm::cross((B - A), (C - A));
    surfaceNormal = glm::normalize(surfaceNormal);
}

Triangle::~Triangle()
{
    verticies.clear();
    delete bbox;
}

bool Triangle::isClosestHit(const Ray &ray, const float &tMin, float &tMax, RaycastHit &hit)
{
    // check the bounding box first
    if (!bbox->isClosestHit(ray, tMin, tMax, hit))
    {
        return false;
    }

    auto rDir = ray.getDirection();
    auto rOr = ray.getOrigin();

    float a = A[0] - B[0];
    float b = A[1] - B[1];
    float c = A[2] - B[2];
    float d = A[0] - C[0];
    float e = A[1] - C[1];
    float f = A[2] - C[2];
    float g = rDir[0];
    float h = rDir[1];
    float i = rDir[2];
    float j = A[0] - rOr[0];
    float k = A[1] - rOr[1];
    float l = A[2] - rOr[2];

    float AKMinusJB = a*k - j*b;
    float JCMinusAL = j*c - a*l;
    float BLMinusKC = b*l - k*c;
    float EIMinusHF = e*i - h*f;
    float GFMinusDI = g*f - d*i;
    float DHMinusEG = d*h - e*g;

    float invM = 1.0f / (a*EIMinusHF + b*GFMinusDI + c*DHMinusEG);

    float t = -1.0f * (f*AKMinusJB + e*JCMinusAL + d*BLMinusKC) * invM;
    if (t < tMin || t > tMax)
    {
        return false;
    }

    float gamma = (i*AKMinusJB + h*JCMinusAL + g*BLMinusKC) * invM;
    if (gamma < 0.0f || gamma > 1.0f)
    {
        return false;
    }

    float beta = (j*EIMinusHF + k*GFMinusDI + l*DHMinusEG) * invM;
    if (beta < 0.0f || beta > 1.0f - gamma)
    {
        return false;
    }

    // if this point is reached, we have intersection
    tMax = t;
    float alpha = 1.0f - beta - gamma;
    auto p = alpha*A + beta*B + gamma*C; // point of intersection

    // set hit struct properties
    hit.normal = surfaceNormal;
    hit.point = p;
    hit.shape = this;
    hit.shader = shaderRef;
    hit.sourceRayDir = ray.getDirection();

    return true;
}
