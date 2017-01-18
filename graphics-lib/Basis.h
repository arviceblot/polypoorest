#pragma once

#include <glm/vec3.hpp>

class Basis
{
public:
    Basis()
        : u(1.0f, 0.0f, 0.0f), v(0.0f, 1.0f, 0.0f), w(0.0f, 0.0f, 1.0f) {}
    Basis(const glm::vec3 &gaze, const glm::vec3 &b = glm::vec3(0.0f, 1.0f, 0.0f));
    virtual ~Basis() {}

    inline const glm::vec3 & getU() const
    {
        return u;
    }
    inline const glm::vec3 & getV() const
    {
        return v;
    }
    inline const glm::vec3 & getW() const
    {
        return w;
    }

    inline void setU(const glm::vec3 &newU)
    {
        u = newU;
    }
    inline void setV(const glm::vec3 &newV)
    {
        v = newV;
    }
    inline void setW(const glm::vec3 &newW)
    {
        w = newW;
    }

protected:
    glm::vec3 u;
    glm::vec3 v;
    glm::vec3 w;
};
