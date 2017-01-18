#pragma once

#include <boost/thread.hpp>
#include "Scene.h"

/**
 * TODO: This could probably be a struct.
 */
class ThreadData
{
public:
    ThreadData(float* image, Scene* scene)
        : scene(scene), image(image)
    {
    }

    Scene *scene;
    unsigned long index;
    int numCpus;
    unsigned long end;
    unsigned long w;
    boost::mutex workMutex;
    float* image;

private:
};
