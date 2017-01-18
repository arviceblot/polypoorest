#pragma once

#include <cmath>
#include "ThreadData.h"

/**
 * The renderWorkerThread is responsible for a portion of the image data and
 * assigning pixel RGB colors to that portion of the image.
 */
class RenderWorkerThread {
public:
    RenderWorkerThread(ThreadData* tdPtr, unsigned long index)
        : threadData(tdPtr), index(index) {}

    void operator()()
    {
        while (index < threadData->end)
        {
            Ray r;
            glm::vec3 color = glm::vec3(0.0f);
            int x = index % threadData->w;
            int y = index / threadData->w;

            int rpp = threadData->scene->args->rpp;
            float N = std::floor(std::sqrt(rpp));

            for (int p = 0; p < N; p++)
            {
                for (int q = 0; q < N; q++)
                {
                    // 1. Compute Viewing Ray
                    threadData->scene->computeRay(
                        r,
                        x + (p + drand48())/N,
                        y + (q + drand48())/N
                    );

                    // 2. Set pixel color based on object shader
                    color += threadData->scene->computeColor(r);
                }
            }

            for (int i = 0; i < 3; i++)
            {
                color[i] = color[i] / std::pow(N, 2);
            }

            for (int i = 0; i < 3; i++)
            {
                threadData->image[i + (x + y * static_cast<int>(threadData->w)) * 3] = color[i];
            }
            threadData->workMutex.lock();
            threadData->index++;
            index = threadData->index;
            threadData->workMutex.unlock();
        }
    }

private:
    ThreadData *threadData;
    unsigned long index;
};
