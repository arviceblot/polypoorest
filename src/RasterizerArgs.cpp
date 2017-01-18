#include "RasterizerArgs.h"

RasterizerArgs::RasterizerArgs() : GraphicsArgs(), rasterize(false) {
    reg("rasterize", "use the rasterizer", ArgumentParsing::NONE, 'R');
}

void RasterizerArgs::process(int argc, char* argv[]) {
    GraphicsArgs::process(argc, argv);

    rasterize = isSet("rasterize");
    if (verbose && rasterize) {
        std::cout << "Using rasterizer" << std::endl;
    }
}
