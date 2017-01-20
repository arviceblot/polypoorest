#include "Shader.h"

#include <glm/gtc/constants.hpp>

#include "Mathf.h"
#include "Scene.h"

Shader::~Shader()
{
}

glm::vec3 Shader::apply(Scene *scene, RaycastHit &hit, int depth)
{
    glm::vec3 c = glm::vec3(0.0f);
    int samples = scene->args->rpp;

    // loop over all the lights in the scene
    for (auto light : scene->lights)
    {
        perLightOperation(scene, light, hit, depth, c);
    }

    postLightingOperation(scene, hit, depth, c);

    // calculate indirect lighting :3
    glm::vec3 indirect = glm::vec3(0.0f);
    float pdf = 1.0f / (2.0f * glm::pi<float>());

    // how many rays we use for sampling
    int giRays = 16;

    for (int i = 0; i < giRays; i++)
    {
        // generate the hemisphere ray using the hit normal?
        auto sample = Mathf::UniformSampleHemisphere();
    }

    // clamp the colors
    for (int i = 0; i < 3; i++)
    {
        c[i] = std::min(1.0f, std::max(0.0f, c[i]));
    }
    return c;
}

void Shader::perLightOperation(Scene *scene, Light *light, RaycastHit & hit, int depth, glm::vec3 &color)
{
}


void Shader::postLightingOperation(Scene *scene, RaycastHit & hit, int depth, glm::vec3 &color)
{
}
