#include "Box.h"
#include "BoundingBox.h"
#include "Triangle.h"

Box::Box(
    const std::string &name,
    const std::string &type,
    Shader *shaderRef,
    const glm::vec3 &minPoint,
    const glm::vec3 &maxPoint
) : Shape(name, type, shaderRef)
{
    // create the bounding box
    bbox = new BoundingBox(minPoint, maxPoint);

    // create the verticies from the min/max points
    auto a = glm::vec3(minPoint[0], maxPoint[1], minPoint[2]);
    auto b = glm::vec3(maxPoint[0], maxPoint[1], minPoint[2]);
    auto c = glm::vec3(maxPoint[0], minPoint[1], minPoint[2]);
    auto d = glm::vec3(maxPoint[0], minPoint[1], maxPoint[2]);
    auto e = glm::vec3(minPoint[0], minPoint[1], maxPoint[2]);
    auto f = glm::vec3(minPoint[0], maxPoint[1], maxPoint[2]);

    // add them to the list
    std::vector<glm::vec3> vert;
    vert.push_back(a);         // 0
    vert.push_back(b);         // 1
    vert.push_back(c);         // 2
    vert.push_back(d);         // 3
    vert.push_back(e);         // 4
    vert.push_back(f);         // 5
    vert.push_back(minPoint);  // 6
    vert.push_back(maxPoint);  // 7

    // make the triangles from the verticies
    triangles.push_back(new Triangle("1", "triangle", shaderRef, vert[6], vert[2], vert[0]));
    triangles.push_back(new Triangle("2", "triangle", shaderRef, vert[0], vert[2], vert[1]));
    triangles.push_back(new Triangle("3", "triangle", shaderRef, vert[7], vert[3], vert[1]));
    triangles.push_back(new Triangle("4", "triangle", shaderRef, vert[1], vert[3], vert[2]));
    triangles.push_back(new Triangle("5", "triangle", shaderRef, vert[4], vert[5], vert[0]));
    triangles.push_back(new Triangle("6", "triangle", shaderRef, vert[6], vert[4], vert[0]));
    triangles.push_back(new Triangle("7", "triangle", shaderRef, vert[3], vert[4], vert[6]));
    triangles.push_back(new Triangle("8", "triangle", shaderRef, vert[3], vert[6], vert[2]));
    triangles.push_back(new Triangle("9", "triangle", shaderRef, vert[4], vert[3], vert[5]));
    triangles.push_back(new Triangle("10", "triangle", shaderRef, vert[5], vert[3], vert[7]));
    triangles.push_back(new Triangle("11", "triangle", shaderRef, vert[0], vert[5], vert[1]));
    triangles.push_back(new Triangle("12", "triangle", shaderRef, vert[5], vert[7], vert[1]));
}

Box::~Box()
{
    // delete all the triangles
    if (!triangles.empty())
    {
        for (std::vector<Triangle*>::iterator it = triangles.begin(); it != triangles.end();)
        {
            delete (*it);
            it = triangles.erase(it);
        }
    }

    verticies.clear();
    delete bbox;
}

bool Box::isClosestHit(const Ray &ray, const float &tMin, float &tMax, RaycastHit &hit)
{
    // check the bounding box first
    if (!bbox->isClosestHit(ray, tMin, tMax, hit))
    {
        return false;
    }

    bool closestHit = false;
    for (auto triangle : triangles)
    {
        if (triangle->isClosestHit(ray, tMin, tMax, hit))
        {
            closestHit = true;
        }
    }
    return closestHit;
}
