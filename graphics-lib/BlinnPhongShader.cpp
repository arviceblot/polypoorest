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

    if (!scene->isHit(shadowRay, 0.0001f, 1.0f))
    {
        shadowRay.setDirection(glm::normalize(shadowRay.getDirection()));

        glm::vec3 v = scene->cameras[0]->getPosition() - hit.point;
        v = glm::normalize(v);

        // bisector between l and v
        glm::vec3 h = (v + shadowRay.getDirection()) / glm::length(v + shadowRay.getDirection());

        c += (diffuse * light->getIntensity()
              * std::max(0.0f, glm::dot(hit.normal, shadowRay.getDirection())))
             + (specular * light->getIntensity()
                * std::pow(std::max(0.0f, glm::dot(hit.normal, h)), phongExponent));
    }
}

void BlinnPhongShader::postLightingOperation(Scene *scene, RaycastHit & hit, int depth, glm::vec3 &color)
{
    // check if we should do anything with reflections
    if (depth > 0 && !Mathf::areLike(mirrorCoefficient, 0.0f))
    {
        // generate reflection ray R
        auto direction = hit.sourceRayDir + 2.0f * glm::dot((-1.0f * hit.sourceRayDir), hit.normal) * hit.normal;
        direction = glm::normalize(direction);
        Ray reflectionRay(hit.point, direction);

        if (!Mathf::areLike(roughness, 0.0f))
        {
            Basis b(reflectionRay.getDirection());
            float u = (-roughness / 2.0f)
                      + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * roughness;
            float v = (-roughness / 2.0f)
                      + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * roughness;
            glm::vec3 rNewDir = reflectionRay.getDirection();
            rNewDir += (u * b.getU()) + (v * b.getV());
            rNewDir = glm::normalize(rNewDir);
            reflectionRay.setDirection(rNewDir);
        }
        color = (1.0f - mirrorCoefficient) * color
            + mirrorCoefficient * scene->computeColor(reflectionRay, 0.0001f, std::numeric_limits<float>::max(), depth - 1);
    }
}
