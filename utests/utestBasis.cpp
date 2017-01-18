/**
 * utestBasis.cpp
 */

#include <cstdlib>
#include "Vector3D.h"
#include "Basis.h"

using namespace sivelab;

int main(int argc, char* argv[]) {
    Vector3D(0, 0, 0) - Vector3D(0, 0, 0);
    Basis a;
    Vector3D u = a.getU();
    Vector3D v = a.getV();
    Vector3D w = a.getW();

    if (u[0] + 0.00001f >= 1.0f && u[0] - 0.00001f <= 1.0f
        && v[1] + 0.00001f >= 1.0f && v[1] - 0.00001f <= 1.0f
        && w[2] + 0.00001f >= 1.0f && w[2] - 0.00001f <= 1.0f)
    {
        exit(EXIT_SUCCESS);
    }
    else {
        exit(EXIT_FAILURE);
    }
}
