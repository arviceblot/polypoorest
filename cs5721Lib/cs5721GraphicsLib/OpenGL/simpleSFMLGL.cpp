#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <limits>
#include <algorithm>
#include <stack>
#include <vector>

#include <GL/glew.h>
#include <SFML/Window.hpp>

#include "handleGraphicsArgs.h"
#include "OpenGLRender.h"

int main(int argc, char *argv[])
{
  sivelab::GraphicsArgs args;
  args.process(argc, argv);

  OpenGLRender *oglHandle = 0;
  oglHandle = new OpenGLRender(args.width, args.height);
  oglHandle->run();

  delete oglHandle;

  exit(EXIT_SUCCESS);
}




