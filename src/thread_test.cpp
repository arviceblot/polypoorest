/**
 * main.cpp
 */

#include <iostream>
#include <cstdlib>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "png++/png.hpp"
#include "handleGraphicsArgs.h"
#include "Vector3D.h"
#include "Basis.h"
#include "RenderWorkerThread.h"
#include "Scene.h"
#include "ThreadData.h"
#include "Util.h"

using namespace sivelab;

int main(int argc, char* argv[]) {
    GraphicsArgs args;
    args.process(argc, argv);

    const int w = args.width;
    const int h = args.height;
    png::image<png::rgb_pixel> imData(w, h);

    // create four threads
    ThreadData* threadDataPtr = new ThreadData(&imData);
    if (args.numCpus == -1) {
        threadDataPtr->numCpus = boost::thread::hardware_concurrency();
    }
    else {
        threadDataPtr->numCpus = args.numCpus;
    }

    const int threads = threadDataPtr->numCpus;
    std::cout << "Threads: " << threads << std::endl;

    unsigned long start = 0;
    unsigned long increment = (w * h) / threads;
    std::vector<boost::thread*> activatedThreads(threads);
    for (int i = 0; i < threads; i++) {
        // if it's the last thread, make sure it gets all the pixels
        if (i == threads - 1) {
            RenderWorkerThread rwt(threadDataPtr, start, (w * h) - 1, w);
            activatedThreads[i] = new boost::thread(rwt);
        }
        else {
            RenderWorkerThread rwt(threadDataPtr, start, start + increment, w);
            activatedThreads[i] = new boost::thread(rwt);
        }
        start += increment;
    }

    // wait for threads to finish
    for (int i = 0; i < threads; i++) {
        activatedThreads[i]->join();
    }

    // write the file
    if (args.outputFileName != "") {
        imData.write(args.outputFileName);
    }
    else {
        imData.write("test.png");
    }

    exit(EXIT_SUCCESS);
}
