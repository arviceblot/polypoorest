#pragma once

#include <memory>
#include <string>
#include <glm/vec3.hpp>
#include "Light.h"
#include "SceneElement.h"

// forward declaration
class Scene;
class RaycastHit;

class Shader : public SceneElement
{
public:
    Shader(const std::string &name, const std::string &type, const glm::vec3 &diffuse = glm::vec3(0.0f));

    glm::vec3 apply(Scene *scene, RaycastHit &hit, int depth);

    const glm::vec3 & getDiffuse() const;

protected:
    /** This should be implemented by child classes. */
    virtual void perLightOperation(Scene *scene, std::shared_ptr<Light> light, RaycastHit & hit, int depth, glm::vec3 &color) = 0;
    virtual void postLightingOperation(Scene *scene, RaycastHit & hit, int depth, glm::vec3 &color) = 0;

    glm::vec3 diffuse;

private:
    Shader();
};

inline Shader::Shader(const std::string &name, const std::string &type, const glm::vec3 &diffuse)
    : SceneElement(name, type, SceneElement::Type::SHADER), diffuse(diffuse) {}

inline const glm::vec3 & Shader::getDiffuse() const
{
    return diffuse;
}
