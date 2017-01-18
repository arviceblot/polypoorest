/**
 * utestMatrix4x4.h
 */

#include <cstdlib>
#include <iostream>
#include "Vector3D.h"
#include "Matrix4x4.h"

using namespace std;
using namespace sivelab;

int main(int argc, char* argv[]) {
    Matrix4x4 test1;
    cout << "Default" << endl;
    test1.print();

    cout << "I" << endl;
    test1.setIdentity();
    test1.print();

    cout << "I * I" << endl;
    test1 = test1 * test1;
    test1.print();

    Matrix4x4 test2(2, 0, 0, 0,
                    0, 2, 0, 0,
                    0, 0, 2, 0,
                    0, 0, 0, 2);
    Matrix4x4 test3(3, 0, 0, 0,
                    0, 3, 0, 0,
                    0, 0, 3, 0,
                    0, 0, 0, 3);
    test2 = test2 * test3;
    cout << "2I * 3I" << endl;
    test2.print();

    double w = 0.5f;
    Vector3D halfResult = test2.multVector(Vector3D(0.5f, 0.5f, 0.5f), w);
    cout << "Half" << endl
        << halfResult << " " << w << endl;

    cout << "Determinant" << endl
        << test2.det() << endl;

    exit(EXIT_SUCCESS);
}
