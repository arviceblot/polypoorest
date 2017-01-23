#include "Scene.h"
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include "RaycastHit.h"
#include "LambertianShader.h"
#include "PerspectiveCamera.h"
#include "SceneElement.h"
#include "Triangle.h"
#include "Mathf.h"

using namespace sivelab;

std::shared_ptr<Shape> Scene::lastHit = NULL;


void Scene::add(std::shared_ptr<Camera> e)
{
    if (e != NULL && e->getSceneElementType() == SceneElement::Type::CAMERA)
    {
        // make sure we set the size, this can probably be moved elsewhere
        e->setSize(glm::vec2(args->width, args->height));
        cameras.push_back(e);
    }
}

void Scene::add(std::shared_ptr<Light> e)
{
    if (e != NULL && e->getSceneElementType() == SceneElement::Type::LIGHT)
    {
        lights.push_back(e);
    }
}

void Scene::add(std::shared_ptr<Shader> e)
{
    if (e != NULL && e->getSceneElementType() == SceneElement::Type::SHADER)
    {
        shaders[e->getName()] = e;
    }
}

void Scene::add(std::shared_ptr<Shape> e)
{
    if (e != NULL && e->getSceneElementType() == SceneElement::Type::SHAPE)
    {
        shapes.push_back(e);
    }
}

void Scene::addBase(std::shared_ptr<Shape> e)
{
    if (e != NULL && e->getSceneElementType() == SceneElement::Type::SHAPE)
    {
        bases[e->getName()] = e;
    }
}

void Scene::buildBVH()
{
    root = std::make_shared<BVHNode>(shapes, 1);
}

std::shared_ptr<Shader> Scene::getShader(const std::string &ref)
{
    auto it = shaders.find(ref);
    if (it == shaders.end())
    {
        std::cerr << "Shader: " << ref << " not found!" << std::endl;
        return NULL;
    }
    else
    {
        return it->second;
    }
}

std::shared_ptr<Shape> Scene::getInstance(const std::string &ref)
{
    auto it = bases.find(ref);
    if (it == bases.end())
    {
        std::cerr << "Refenced Shape: " << ref << " not found!" << std::endl;
        return NULL;
    }
    else
    {
        return it->second;
    }
}

void Scene::computeRay(Ray &ray, float i, float j)
{
    cameras[0]->computeRay(ray, i, j);
}

bool Scene::isHit(const Ray& r, float tMin, float tMax)
{
    RaycastHit hit;

    // check last hit cache
    if (lastHit != NULL)
    {
        if (lastHit->isClosestHit(r, tMin, tMax, hit))
        {
            return true;
        }
    }

    if (root->isClosestHit(r, tMin, tMax, hit))
    {
        lastHit = hit.shape;
        return true;
    }
    return false;
}

glm::vec3 Scene::computeColor(Ray &r, float tMin, float tMax, int depth)
{
    if (depth == -1)
    {
        depth = args->recursionDepth;
    }

    // should set default color to background
    auto color = ambient;

    if (depth > 0)
    {
        RaycastHit hit;
        if (root->isClosestHit(r, tMin, tMax, hit))
        {
            color = hit.shader->apply(this, hit, depth);
        }
    }
    return color;
}
