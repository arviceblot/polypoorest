#pragma once

#include "SceneDataContainer.h"
#include "Scene.h"

using namespace sivelab;

class ShaderCreator : public SceneElementCreator
{
public:
    ShaderCreator(Scene *scene);
    virtual ~ShaderCreator();

    void instance(const ptree::value_type& v);

private:
    Scene *scene;
};
