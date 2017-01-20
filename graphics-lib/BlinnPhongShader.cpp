#include "BlinnPhongShader.h"
#include <cmath>
#include <limits>
#include <glm/glm.hpp>
#include "RaycastHit.h"
#include "Light.h"
#include "Scene.h"
#include "Mathf.h"

void BlinnPhongShader::perLightOperation(Scene *scene, Light *light, RaycastHit &hit, int depth, glm::vec3 &c)
{
    // generate shadow ray s to light from hit structure
    // Note: this is L in our equation
    Ray shadowRay(hit.point, light->getPosition() - hit.point);

    if (!scene->isHit(shadowRay, Mathf::Epsilon, 1.0f))
    {
        shadowRay.direction = glm::normalize(shadowRay.direction);

        glm::vec3 v = scene->cameras[0]->getPosition() - hit.point;
        v = glm::normalize(v);

        // bisector between l and v
        glm::vec3 h = (v + shadowRay.direction) / glm::length(v + shadowRay.direction);

        c += (diffuse * light->getIntensity() * std::max(0.0f, glm::dot(hit.normal, shadowRay.direction)))
             + (specular * light->getIntensity() * std::pow(std::max(0.0f, glm::dot(hit.normal, h)), phongExponent));
    }
}

void BlinnPhongShader::postLightingOperation(Scene *scene, RaycastHit & hit, int depth, glm::vec3 &color)
{
    // check if we should do anything with reflections
    if (depth > 0 && !Mathf::Approximately(mirrorCoefficient, 0.0f))
    {
        // generate reflection ray R
        auto direction = hit.sourceRayDir + 2.0f * glm::dot((-1.0f * hit.sourceRayDir), hit.normal) * hit.normal;
        direction = glm::normalize(direction);
        Ray reflectionRay(hit.point, direction);

        if (!Mathf::Approximately(roughness, 0.0f))
        {
            Basis b(reflectionRay.direction);
            float u = (-roughness / 2.0f) + Mathf::Random() * roughness;
            float v = (-roughness / 2.0f) + Mathf::Random() * roughness;
            glm::vec3 rNewDir = reflectionRay.direction;
            rNewDir += (u * b.getU()) + (v * b.getV());
            rNewDir = glm::normalize(rNewDir);
            reflectionRay.direction = rNewDir;
        }
        color = (1.0f - mirrorCoefficient) * color
                + mirrorCoefficient * scene->computeColor(reflectionRay, Mathf::Epsilon, Mathf::Max, depth - 1);
    }
}
