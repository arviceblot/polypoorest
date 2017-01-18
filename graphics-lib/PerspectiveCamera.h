#pragma once

#include <string>
#include <glm/vec3.hpp>
#include "Basis.h"
#include "Camera.h"
#include "Ray.h"

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(
        const std::string &name,
        const std::string &type,
        const glm::vec3 &position = glm::vec3(0.0f),
        const glm::vec3 &viewDir  = glm::vec3(0.0f, 0.0f, -1.0f),
        float focalLength = 1.0f,
        float imagePlaneWidth = 0.5f,
        float aspectRatio = 1.0f)
        : Camera(name, type, position, viewDir, focalLength, imagePlaneWidth, aspectRatio) {}
    virtual ~PerspectiveCamera() {}

    virtual void computeRay(Ray &r, float i, float j);

private:
};
