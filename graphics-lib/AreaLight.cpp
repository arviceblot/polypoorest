#include "AreaLight.h"
#include <cstdlib>

AreaLight::AreaLight(
    const std::string &type,
    const glm::vec3 &position,
    const glm::vec3 &intensity,
    const glm::vec3 &normal,
    const float &width,
    const float &length
) : Light(type, position, intensity),
    basis(normal, glm::vec3(1.0f, 0.0f, 0.0f)),
    width(width),
    length(length)
{
    basis.setU(basis.getU() * width * 0.5f);
    basis.setV(basis.getV() * length * 0.5f);
}

AreaLight::~AreaLight()
{
}

/**
 * WTF is this doing?!
 */
const glm::vec3 & AreaLight::getPosition() const
{
    auto result = position;
    float e1 = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    result += e1 * basis.getU();
    float e2 = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    result += e2 * basis.getV();
    return result;
}
