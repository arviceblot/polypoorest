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
#include "Mathf.h"
#include "Shader.h"
#include "Shape.h"
#include "SceneElement.h"

using namespace sivelab;

class Scene
{
public:
    Scene(const GraphicsArgs *args);

    void add(std::shared_ptr<Camera> e);
    void add(std::shared_ptr<Light> e);
    void add(std::shared_ptr<Shader> e);
    void add(std::shared_ptr<Shape> e);
    void addBase(std::shared_ptr<Shape> e);
    void buildBVH();

    std::shared_ptr<Shader> getShader(const std::string &ref);
    std::shared_ptr<Shape> getInstance(const std::string &ref);
    void computeRay(Ray &ray, float i, float j);
    bool isHit(const Ray &r, float tMin = 1.0001f, float tMax = Mathf::Max);
    glm::vec3 computeColor(Ray &r, float tMin = 1.0001f, float tMax = Mathf::Max, int depth = -1);
    void rasterize(png::image<png::rgb_pixel>* image);

    const GraphicsArgs *args;
    std::map<std::string, std::shared_ptr<Shape>> bases; // used for instanced objects?
    std::vector<std::shared_ptr<Camera>> cameras;
    std::vector<std::shared_ptr<Light>> lights;
    std::map<std::string, std::shared_ptr<Shader>> shaders;
    std::vector<std::shared_ptr<Shape>> shapes;
    std::shared_ptr<BVHNode> root;

    glm::vec3 ambient;

private:
    static std::shared_ptr<Shape> lastHit;
};

inline Scene::Scene(const GraphicsArgs *args)
    : args(args)
{
    ambient = glm::vec3(0.1f);
}
