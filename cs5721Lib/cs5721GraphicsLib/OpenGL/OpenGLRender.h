#ifndef __TERRAIN_GL_VIEW_H__
#define __TERRAIN_GL_VIEW_H__

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

#include "GLSL.h"

class OpenGLRender
{  
public:   
  
  OpenGLRender(const int winWidth, const int winHeight);
  ~OpenGLRender() {}

  void initializeShader( const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename );
  void run();
       
private:  

  int m_winWidth, m_winHeight;
  sf::Window *m_Window;

  // Vertex Buffer Object related variables
  int m_numVertices;
  GLuint m_triangleVBO;

  // Determines whether we'll use shaders in this example or not.
  bool m_useShaders;
  sivelab::GLSLObject simpleShader;
};  

#endif
