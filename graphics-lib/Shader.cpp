#include "Shader.h"
#include "Scene.h"

Shader::~Shader()
{
}

glm::vec3 Shader::apply(Scene *scene, RaycastHit &hit, int depth)
{
    glm::vec3 c = glm::vec3(0.0f);
    int samples = scene->args->rpp; // TODO: change this to raysPerLight

    // loop over all the lights in the scene
    for (auto light : scene->lights)
    {
        for (int s = 0; s < samples; s++)
        {
            perLightOperation(scene, light, hit, depth, c);
        }

        // average for samples
        for (int i = 0; i < 3; i++)
        {
            c[i] /= static_cast<float>(samples);
        }
    }

    postLightingOperation(scene, hit, depth, c);

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
