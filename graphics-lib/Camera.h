#pragma once

#include <string>
#include <glm/glm.hpp>
#include "Basis.h"
#include "Ray.h"
#include "SceneElement.h"


class Camera : public SceneElement
{
public:
    Camera(const std::string &name,
           const std::string &type,
           const glm::vec3 &position,
           const glm::vec3 &viewDir,
           const float &focalLength,
           const float &imagePlaneWidth,
           const float &aspectRatio);

    inline const Basis & getBasis() const;

    inline const glm::vec3 & getPosition() const;

    inline const glm::vec2 & getSize() const;

    inline const float & getFocalLength() const;

    inline const float & getImagePlaneWidth() const;

    inline const float & getAspectRatio() const;

    inline void setSize(const glm::vec2 &newSize);

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

inline Camera::Camera(const std::string &name,
                      const std::string &type,
                      const glm::vec3 &position,
                      const glm::vec3 &viewDir,
                      const float &focalLength,
                      const float &imagePlaneWidth,
                      const float &aspectRatio)
    : SceneElement(name, type, SceneElement::Type::CAMERA),
      position(position), basis(viewDir), focalLength(focalLength),
      imagePlaneWidth(imagePlaneWidth), aspectRatio(aspectRatio) {}

inline const Basis & Camera::getBasis() const
{
    return basis;
}

inline const glm::vec3 & Camera::getPosition() const
{
    return position;
}

inline const glm::vec2 & Camera::getSize() const
{
    return size;
}

inline const float & Camera::getFocalLength() const
{
    return focalLength;
}

inline const float & Camera::getImagePlaneWidth() const
{
    return imagePlaneWidth;
}

inline const float & Camera::getAspectRatio() const
{
    return aspectRatio;
}

inline void Camera::setSize(const glm::vec2 &newSize)
{
    size = newSize;
}
