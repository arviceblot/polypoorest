#pragma once

#include <string>
#include <glm/glm.hpp>
#include "Basis.h"
#include "Ray.h"
#include "SceneElement.h"


class Camera : public SceneElement
{
public:
    Camera(
        const std::string &name,
        const std::string &type,
        const glm::vec3 &position,
        const glm::vec3 &viewDir,
        const float &focalLength,
        const float &imagePlaneWidth,
        const float &aspectRatio
    ) : SceneElement(name, type, SceneElement::Type::CAMERA),
        position(position), basis(viewDir), focalLength(focalLength),
        imagePlaneWidth(imagePlaneWidth), aspectRatio(aspectRatio) {}
    virtual ~Camera() = 0;

    inline const Basis & getBasis() const
    {
        return basis;
    }
    inline const glm::vec3 & getPosition() const
    {
        return position;
    }
    inline const glm::vec2 & getSize() const
    {
        return size;
    }
    inline const float & getFocalLength() const
    {
        return focalLength;
    }
    inline const float & getImagePlaneWidth() const
    {
        return imagePlaneWidth;
    }
    inline const float & getAspectRatio() const
    {
        return aspectRatio;
    }

    inline void setSize(const glm::vec2 &newSize)
    {
        size = newSize;
    }

    virtual void computeRay(Ray &r, float i, float j) = 0;

protected:
    glm::vec3 position;
    glm::vec2 size;
    Basis basis;
    float focalLength;
    float imagePlaneWidth;
    float aspectRatio;

private:
    Camera();
};
