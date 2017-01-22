/**
 * ray-tracer.cpp
 */

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "png++/png.hpp"
#include "XMLSceneParser.h"
#include "CameraCreator.h"
#include "LightCreator.h"
#include "RasterizerArgs.h"
#include "RenderWorkerThread.h"
#include "ShaderCreator.h"
#include "ShapeCreator.h"
#include "Scene.h"
#include "ThreadData.h"
#include "RenderWindow.h"

using namespace sivelab;

int main(int argc, char* argv[])
{
    // process CLI args
    RasterizerArgs args;
    args.process(argc, argv);

    const int w = args.width;
    const int h = args.height;
    png::image<png::rgb_pixel> imData(w, h);
    XMLSceneParser xmlScene;
    Scene *scene = new Scene(&args);

    // register object creation function with xlmScene
    xmlScene.registerCallback("camera", new CameraCreator(scene));
    xmlScene.registerCallback("light", new LightCreator(scene)); // let there be light
    xmlScene.registerCallback("shader", new ShaderCreator(scene));
    xmlScene.registerCallback("shape", new ShapeCreator(scene));
    xmlScene.registerCallback("instance", new ShapeCreator(scene));

    // parse the scene
    if (args.inputFileName != "")
    {
        std::cout << "Parsing scene..." << std::endl;
        xmlScene.parseFile(args.inputFileName);
    }
    else
    {
        std::cerr << "Need input file!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // build the BVH
    std::cout << "Building BVH..." << std::endl;
    scene->buildBVH();

    // allocate image
    float* image = new float[3 * w * h];

    ThreadData* threadData = new ThreadData(image, scene);

    // figure out num threads
    if (args.numCpus == -1)
    {
        threadData->numCpus = boost::thread::hardware_concurrency();
    }
    else
    {
        threadData->numCpus = args.numCpus;
    }

    threadData->w = w;
    threadData->end = w * h;
    threadData->index = threadData->numCpus - 2;

    const int threads = threadData->numCpus + 1; // one more for render window
    std::cout << "Threads: " << threads << std::endl;

    // store threads here
    boost::thread *activatedThreads[threads];

    // create ray-tracing threads
    std::cout << "Creating threads..." << std::endl;
    for (int i = 0; i < threads - 1; i++)
    {
        RenderWorkerThread rwt(threadData, i);
        activatedThreads[i] = new boost::thread(rwt);
    }

    std::cout << "Creating render window..." << std::endl;
    RenderWindow window("polypoorest", threadData->image, args.width, args.height);
    activatedThreads[threads - 1] = new boost::thread(window);

    // wait for threads to finish
    std::cout << "Rendering..." << std::endl;
    for (int i = 0; i < threads - 1; i++)
    {
        activatedThreads[i]->join();
    }

    // move the color buff to the image data
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            imData[y][x] = png::rgb_pixel(
                               static_cast<int>(std::floor(threadData->image[(x + y * w) * 3] * 255)),
                               static_cast<int>(std::floor(threadData->image[1 + (x + y * w) * 3] * 255)),
                               static_cast<int>(std::floor(threadData->image[2 + (x + y * w) * 3] * 255))
                           );
        }
    }

    // write the file
    std::cout << "Writing file... ";
    if (args.outputFileName != "")
    {
        imData.write(args.outputFileName);
    }
    else
    {
        std::stringstream s;
        s << std::time(0) << ".png";
        imData.write(s.str());
    }

    std::cout << "finished" << std::endl;

    // wait for render window close
    std::cout << "Awaiting window close..." << std::endl;
    activatedThreads[threads - 1]->join();

    delete threadData;
    delete [] image;

    exit(EXIT_SUCCESS);
}
