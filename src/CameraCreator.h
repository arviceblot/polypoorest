#pragma once

#include "SceneDataContainer.h"
#include "Scene.h"

using namespace sivelab;

class CameraCreator : public SceneElementCreator
{
public:
    CameraCreator(Scene *scene) : scene(scene) {}
    virtual ~CameraCreator() {}

    void instance(const ptree::value_type &v);

private:
    Scene *scene;
};
