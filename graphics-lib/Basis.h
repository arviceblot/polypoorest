#pragma once

#include <glm/vec3.hpp>

class Basis
{
public:
    Basis();
    Basis(const glm::vec3 &gaze, const glm::vec3 &b = glm::vec3(0.0f, 1.0f, 0.0f));

    const glm::vec3 & getU() const;
    const glm::vec3 & getV() const;
    const glm::vec3 & getW() const;

    void setU(const glm::vec3 &newU);
    void setV(const glm::vec3 &newV);
    void setW(const glm::vec3 &newW);

protected:
    glm::vec3 u;
    glm::vec3 v;
    glm::vec3 w;
};

inline Basis::Basis()
    : u(1.0f, 0.0f, 0.0f), v(0.0f, 1.0f, 0.0f), w(0.0f, 0.0f, 1.0f) {}

inline const glm::vec3 & Basis::getU() const
{
    return u;
}

inline const glm::vec3 & Basis::getV() const
{
    return v;
}

inline const glm::vec3 & Basis::getW() const
{
    return w;
}

inline void Basis::setU(const glm::vec3 &newU)
{
    u = newU;
}

inline void Basis::setV(const glm::vec3 &newV)
{
    v = newV;
}

inline void Basis::setW(const glm::vec3 &newW)
{
    w = newW;
}
