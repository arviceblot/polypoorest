#pragma once

#include "SceneDataContainer.h"
#include "Scene.h"

using namespace sivelab;

class LightCreator : public SceneElementCreator
{
public:
    LightCreator(Scene *scene) : scene(scene) {}
    virtual ~LightCreator() {}

    void instance(const ptree::value_type& v);

private:
    Scene *scene;
};
