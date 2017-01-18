#pragma once

#include <string>
#include <glm/vec3.hpp>
#include "Light.h"
#include "SceneElement.h"

// forward declaration
class Scene;
struct RaycastHit;

class Shader : public SceneElement
{
public:
    Shader(const std::string &name, const std::string &type, const glm::vec3 &diffuse = glm::vec3(0.0f))
        : SceneElement(name, type, SceneElement::Type::SHADER), diffuse(diffuse) {}
    virtual ~Shader() = 0;

    glm::vec3 apply(Scene *scene, RaycastHit &hit, int depth);

    inline const glm::vec3 & getDiffuse() const
    {
        return diffuse;
    }

protected:
    /** This should be implemented by child classes. */
    virtual void perLightOperation(Scene *scene, Light *light, RaycastHit & hit, int depth, glm::vec3 &color) = 0;
    virtual void postLightingOperation(Scene *scene, RaycastHit & hit, int depth, glm::vec3 &color) = 0;

    glm::vec3 diffuse;

private:
    Shader();
};
