#pragma once

#include <glm/glm.hpp>
#include "Shape.h"

class Transform
{
public:
    Transform();

private:
    glm::vec3 translation;
    glm::vec3 scale;
    glm::vec3 rotation;
    /* Full transformation matrix */
    glm::mat4 matrix;
};
