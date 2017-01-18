#ifndef __VBO_H__
#define __VBO_H__ 1

#include <GL/glew.h>
#include "Box.h"

class VBO : public Box {
public:
    VBO(
        GLuint vbo,
        const Vector3D& color = Vector3D(1.0f, 0.0f, 0.0f),
        const Vector3D& minPoint = Vector3D(-0.5f, -0.5f, -0.5f),
        const Vector3D& maxPoint = Vector3D(0.5f, 0.5f, 0.5f)
    );
    virtual ~VBO() {}

    void load();
    void render();

    GLuint vbo;
    Vector3D color;
};

#endif /* VBO.h */
