#include "AreaLight.h"
#include <cstdlib>
#include "Mathf.h"

inline const glm::vec3 AreaLight::getPosition() const
{
    // get a random point inside the area light
    return position + Mathf::Random() * basis.getU() + Mathf::Random() * basis.getV();
}
