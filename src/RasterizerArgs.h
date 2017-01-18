#ifndef __RASTERIZER_ARGS_H__
#define __RASTERIZER_ARGS_H__ 1

#include "handleGraphicsArgs.h"

using namespace sivelab;

class RasterizerArgs : public GraphicsArgs {
public:
    RasterizerArgs();
    ~RasterizerArgs() {}

    void process(int argc, char* argv[]);

    bool rasterize;
};

#endif /* RasterizerArgs.h */
