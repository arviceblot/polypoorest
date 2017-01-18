#pragma once

#include <glm/glm.hpp>
#include "SceneDataContainer.h"
#include "Scene.h"

using namespace sivelab;

class ShapeCreator : public SceneElementCreator {
public:
    ShapeCreator(Scene *scene) : scene(scene) {}
    virtual ~ShapeCreator() {}

    void instance(const ptree::value_type& v);

private:
    const glm::mat4 parseTransform(const ptree::value_type& v);

    Scene *scene;
};
