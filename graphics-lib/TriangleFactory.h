#pragma once

#include "Triangle.h"

class TriangleFactory
{
public:
    static std::shared_ptr<Triangle> Create(const std::string &name,
                                            const std::string &type,
                                            std::shared_ptr<Shader> shaderRef,
                                            const glm::vec3 &a = glm::vec3(0.0f),
                                            const glm::vec3 &b = glm::vec3(0.0f),
                                            const glm::vec3 &c = glm::vec3(0.0f));
};

inline std::shared_ptr<Triangle> TriangleFactory::Create(const std::string &name,
        const std::string &type,
        std::shared_ptr<Shader> shaderRef,
        const glm::vec3 &a,
        const glm::vec3 &b,
        const glm::vec3 &c)
{
    auto triangle = std::make_shared<Triangle>(name, type, shaderRef, a, b, c);
    triangle->finalize();
    return triangle;
}
