#pragma once

#include <cmath>
#include <chrono>
#include <limits>
#include <random>

#include <glm/glm.hpp>

struct Mathf
{
    static constexpr float Epsilon = 0.0001f;

    static constexpr float Max = std::numeric_limits<float>::max();

    static constexpr float Min = std::numeric_limits<float>::min();

    static bool Approximately(const float &a, const float &b);

    static float Random(const float &a = 0.0f, const float &b = 1.0f);

    static glm::vec3 UniformSampleHemisphere();

private:
    static std::random_device rd;
    static std::mt19937 gen;
};


inline bool Mathf::Approximately(const float &a, const float &b)
{
    return (std::abs(a - b) < Epsilon);
}

inline float Mathf::Random(const float &a, const float &b)
{
    std::uniform_real_distribution<float> dis(a, b);
    return dis(gen);
}
