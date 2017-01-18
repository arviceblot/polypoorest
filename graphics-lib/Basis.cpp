#include "Basis.h"

#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtx/vector_query.hpp>

#include "Mathf.h"

Basis::Basis(const glm::vec3 &gaze, const glm::vec3 &b)
{
    //      -gaze
    // W = --------
    //     ||gaze||
    w = (-1.0f * gaze) / glm::length(gaze);

    auto t = b;
    if (glm::areCollinear(w, b, Mathf::pcEps))
    {
        int mindex = 0;
        for (int i = 0; i < 3; i++)
        {
            if (t[mindex] > t[i])
            {
                mindex = i;
            }
        }
        t[mindex] = 1.0f;
    }

    //       bxW
    // U = -------
    //     ||bxW||
    glm::vec3 tCrossW = glm::cross(t, w);
    u = tCrossW / glm::length(tCrossW);

    // V = WxU
    v = glm::cross(w, u);
}
