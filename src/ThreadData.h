#pragma once

#include <boost/thread.hpp>
#include "Scene.h"

class ThreadData
{
public:
    ThreadData(float *image, Scene *scene);

    Scene *scene;
    unsigned long index;
    int numCpus;
    unsigned long end;
    unsigned long w;
    boost::mutex workMutex;
    float *image;
};

inline ThreadData::ThreadData(float* image, Scene* scene)
    : scene(scene), image(image) {}
