#pragma once

namespace Mathf
{
const static float pcEps = 0.00001f;

inline bool areLike(const float &a, const float &b)
{
    return (a + pcEps >= b && a - pcEps <= b);
}
}
