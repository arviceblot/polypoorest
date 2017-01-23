#include "LambertianShader.h"
#include <algorithm>
#include <cmath>
#include "RaycastHit.h"
#include "Light.h"
#include "Scene.h"
#include "Mathf.h"

void LambertianShader::perLightOperation(Scene *scene, std::shared_ptr<Light> light, RaycastHit &hit, int depth, glm::vec3 &color)
{
    // generate shadow ray s to light from hit structure
    auto direction = glm::normalize(light->getPosition() - hit.point);
    Ray shadowRay(hit.point, direction);

    if (!scene->isHit(shadowRay, Mathf::Epsilon))
    {
        color += diffuse * light->getIntensity()
                 * std::max(0.0f, glm::dot(hit.normal, shadowRay.direction));
    }
}


void LambertianShader::postLightingOperation(Scene *scene, RaycastHit & hit, int depth, glm::vec3 &color)
{
    // check if we should do anything with reflections
    if (depth > 0 && !Mathf::Approximately(mirrorCoefficient, 0.0f))
    {
        // generate reflection ray
        auto direction = hit.sourceRayDir + 2 * (glm::dot((glm::vec3(-1.0f) * hit.sourceRayDir), hit.normal)) * hit.normal;
        direction = glm::normalize(direction);
        Ray reflectionRay(hit.point, direction);

        color += mirrorCoefficient
                 * scene->computeColor(reflectionRay, Mathf::Epsilon, Mathf::Max, depth - 1);
    }
}
