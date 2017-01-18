#pragma once

#include <limits>
#include <map>
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include "handleGraphicsArgs.h"
#include "png++/png.hpp"
#include "BVHNode.h"
#include "Camera.h"
#include "Light.h"
#include "Shader.h"
#include "Shape.h"
#include "SceneElement.h"

using namespace sivelab;

class Scene
{
public:
    Scene(const GraphicsArgs *args);
    virtual ~Scene();

    void add(SceneElement *e);
    void addBase(SceneElement *e);
    void buildBVH();

    Shader * getShader(const std::string &ref);
    Shape * getInstance(const std::string &ref);
    void computeRay(Ray &ray, float i, float j);
    bool isHit(
        const Ray &r,
        float tMin = 1.0001f,
        float tMax = std::numeric_limits<float>::max()
    );
    glm::vec3 computeColor(
        Ray &r,
        float tMin = 1.0001f,
        float tMax = std::numeric_limits<float>::max(),
        int depth = -1
    );
    void rasterize(png::image<png::rgb_pixel>* image);

    const GraphicsArgs *args;
    std::map<std::string, Shape *> bases; // used for instanced objects?
    std::vector<Camera *> cameras;
    std::vector<Light *> lights;
    std::map<std::string, Shader *> shaders;
    std::vector<Shape *> shapes;
    BVHNode *root;

    glm::vec3 ambient;

private:
    static Shape *lastHit;
};
