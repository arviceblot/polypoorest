#pragma once

#include <glm/vec3.hpp>
#include "Shader.h"
#include "Shape.h"

/**
 * This class should contain all the information we need for shading operations.
 * To do this we need to know:
 *   What was hit
 *   How it was hit
 *   Where it was hit
 * For this, it should contain:
 *   t
 *   Normal vector at point of intersection
 *   Pointer to object that was intersected
 *   Pointer to shader that should be used
 */
class RaycastHit
{
public:
    RaycastHit();
    RaycastHit(float &t, const glm::vec3 &normal, Shape *shape, Shader *shader);

    float t;
    /* The normal of the surface the ray hit. */
    glm::vec3 normal;
    /* The impact point in world space where the ray hit. */
    glm::vec3 point;
    glm::vec3 sourceRayDir;
    Shape *shape;
    Shader *shader;
};

inline RaycastHit::RaycastHit() {}

inline RaycastHit::RaycastHit(float &t, const glm::vec3 &normal, Shape *shape, Shader *shader)
    : t(t), normal(normal), shape(shape), shader(shader) {}
