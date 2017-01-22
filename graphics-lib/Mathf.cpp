#include "Mathf.h"

#include <glm/gtc/constants.hpp>

constexpr float Mathf::Epsilon;

constexpr float Mathf::Max;

constexpr float Mathf::Min;

std::random_device Mathf::rd;

std::mt19937 Mathf::gen(Mathf::rd());

std::uniform_real_distribution<float> Mathf::mainDis(0.0f, 1.0f);

glm::vec3 Mathf::UniformSampleHemisphere()
{
    auto r1 = Random();
    auto r2 = Random();
    float sinTheta = std::sqrt(1.0f - r1 * r1);
    float phi = 2.0f * glm::pi<float>() * r2;
    float x = sinTheta * cosf(phi);
    float z = sinTheta * sinf(phi);
    return glm::vec3(x, r1, z);
}
