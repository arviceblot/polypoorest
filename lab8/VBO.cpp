#include "VBO.h"
#include <cstdlib>
#include "Triangle.h"

VBO::VBO(
    GLuint vbo,
    const Vector3D& color,
    const Vector3D& minPoint,
    const Vector3D& maxPoint
) : Box("", "", NULL, minPoint, maxPoint), color(color), vbo(vbo) {
    Vector3D a(0, 0, 0);
    Vector3D b(0, 0, 0);
    a = a * b;
}

void VBO::load() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    float* hostVertexBuffer = new float[2 * 3 * 3 * 3 * 3 * 3 * 12];
    int tidx = 0;

    for (std::vector<Triangle*>::iterator it = triangles.begin(); it != triangles.end(); it++) {
        std::vector<Vector3D> verts = (*it)->getVerticies();
        int v = 0;
        for (std::vector<Vector3D>::iterator vit = verts.begin(); vit != verts.end(); vit++, v++) {
            Vector3D normal = (*it)->getSurfaceNormal();
            for (int i = 0; i < 3; i++) {
                hostVertexBuffer[tidx++] = (*vit)[i];
            }
            for (int i = 0; i < 3; i++) {
                hostVertexBuffer[tidx++] = normal[i]; // add the surface normal
            }
            // color
            for (int i = 0; i < 3; i++) {
                hostVertexBuffer[tidx++] = color[i];
            }
            // texture coords
            if (v == 0) {
                hostVertexBuffer[tidx++] = 0.0;
                hostVertexBuffer[tidx++] = 0.0;
            }
            else if (v == 1) {
                hostVertexBuffer[tidx++] = 500;
                hostVertexBuffer[tidx++] = 0.0;
            }
            else {
                hostVertexBuffer[tidx++] = 250;
                hostVertexBuffer[tidx++] = 250;
            }
        }
    }

    int numBytes = 2 * 3 * 3 * 3 * 3 * 3 * 12 * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, numBytes, hostVertexBuffer, GL_STATIC_DRAW);
    delete [] hostVertexBuffer;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::render() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexPointer(3, GL_FLOAT, 11 * sizeof(float), 0);
    glNormalPointer(GL_FLOAT, 11 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
    glColorPointer(3, GL_FLOAT, 11 * sizeof(float), (GLvoid*)(6 * sizeof(float)));
    glTexCoordPointer(2, GL_FLOAT, 11 * sizeof(float), (GLvoid*)(9 * sizeof(float)));

    glDrawArrays(GL_TRIANGLES, 0, 3 * 12);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
