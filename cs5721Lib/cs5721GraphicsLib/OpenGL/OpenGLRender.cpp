#include "OpenGLRender.h"

OpenGLRender::OpenGLRender(const int winWidth, const int winHeight)   
  : m_winWidth( winWidth ), 
    m_winHeight( winHeight )
{  
  std::cout << "OpenGLRender - creating window of (" << m_winWidth << " X " << m_winHeight << ")" << std::endl;

  if (m_useShaders)
    std::cout << "\tShaders will be ENABLED!" << std::endl;

  // Create the main window using the width and height parameters from
  // our command line arguments.
  m_Window = new sf::Window(sf::VideoMode(m_winWidth, m_winHeight, 32), "SFML OpenGL");

  glewInit();

  // ////////////////////////////////////////////////////////////////////////////////////
  // load any geometry you might want here.
  // ////////////////////////////////////////////////////////////////////////////////////

  // ////////////////////////////////////////////////////////////////////////////////////
  // Set the active window before using OpenGL commands. For a
  // single window OpenGL program this really doesn't do much, but
  // if you did have multiple windows, you might need to "render"
  // different things to each window, and thus, you need to make
  // each "window app" active before issuing OpenGL commands.
  m_Window->setActive();

  // Set color and depth clear value
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

  // Enable Z-buffer read and write
  // glEnable(GL_DEPTH_TEST);

  // 
  // Setup the projection
  // 
  glViewport(0, 0, m_winWidth, m_winHeight);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  float verticalFieldOfView = 60.0; // in degrees
  float aspectRatio = m_winWidth/(float)m_winHeight;
  float nearPlaneDist = 1.0;
  float farPlaneDist = 500.0;
  gluPerspective(verticalFieldOfView, aspectRatio, nearPlaneDist, farPlaneDist);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Tell OpenGL which way you've defined your triangles' winding
  glFrontFace(GL_CCW);

  // In this example, we're going to render just a few triangles to
  // show how its done.

  glGenBuffers(1, &m_triangleVBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO);

  m_numVertices = 3;
  int numFloatsPerVertex = 3;
  int numColors = 3;  // for non-material shading
  int numFloatsPerColor = 3; 
  int numTextureCoords = 3;
  int numFloatsPerTextureCoord = 2;

  float* host_VertexBuffer = new float[ m_numVertices * numFloatsPerVertex * numColors * numFloatsPerColor * 2 * 3 ];
  
  int tIdx = 0;

  // ////////////////////////
  // V0
  // The vertex data
  host_VertexBuffer[ tIdx++ ] = 0.0f;      
  host_VertexBuffer[ tIdx++ ] = 2.5f;      
  host_VertexBuffer[ tIdx++ ] = -8.0f;     

  // then it's color
  host_VertexBuffer[ tIdx++ ] = 1.0f;
  host_VertexBuffer[ tIdx++ ] = 0.0f;
  host_VertexBuffer[ tIdx++ ] = 1.0f;

  // then, the texture coordinate if it has one
  host_VertexBuffer[ tIdx++ ] = 0.0;
  host_VertexBuffer[ tIdx++ ] = 0.0;

  // ////////////////////////
  // V1
  // the next vertex data
  host_VertexBuffer[ tIdx++ ] = -2.5f;     
  host_VertexBuffer[ tIdx++ ] = -2.5f;     
  host_VertexBuffer[ tIdx++ ] = -5.0f;     

  // it's color
  host_VertexBuffer[ tIdx++ ] = 0.0f;
  host_VertexBuffer[ tIdx++ ] = 1.0f;
  host_VertexBuffer[ tIdx++ ] = 1.0f;

  // then, the texture coordinate
  host_VertexBuffer[ tIdx++ ] = 1.0;
  host_VertexBuffer[ tIdx++ ] = 0.0;

  // ////////////////////////
  // V2
  // the 3rd vertex
  host_VertexBuffer[ tIdx++ ] = 2.5f;      
  host_VertexBuffer[ tIdx++ ] = -2.5f;     
  host_VertexBuffer[ tIdx++ ] = -5.0f;     

  // it's color
  host_VertexBuffer[ tIdx++ ] = 1.0f;
  host_VertexBuffer[ tIdx++ ] = 1.0f;
  host_VertexBuffer[ tIdx++ ] = 0.0f;

  // then, the texture coordinate
  host_VertexBuffer[ tIdx++ ] = 0.5;
  host_VertexBuffer[ tIdx++ ] = 0.5;

  int numBytes = m_numVertices * numFloatsPerVertex * numColors * numFloatsPerColor * 2 * 3 * sizeof(float);
  glBufferData(GL_ARRAY_BUFFER, numBytes, host_VertexBuffer, GL_STATIC_DRAW);
  delete [] host_VertexBuffer;

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// /////////////////////////////////////////////////////////////////////////
// Use and initialize shaders. This must be called after the
// OpenGLRender class is instanced. The function will turn on GLSL
// shading and load, compile, and link the shaders.
//
void OpenGLRender::initializeShader( const std::string &vertexShaderFilename,
				     const std::string &fragmentShaderFilename )
{
  m_useShaders = true;

  // /////////////////////////////////////////////////////////////////////////
  // 
  // Shader loading - if shaders are enabled by the instancing of the
  // OpenGLRender class, then allocate the shaders here.
  // 
  simpleShader.addShader(vertexShaderFilename, sivelab::GLSLObject::VERTEX_SHADER);
  simpleShader.addShader(fragmentShaderFilename, sivelab::GLSLObject::FRAGMENT_SHADER);
  simpleShader.createProgram();
}

void OpenGLRender::run()  
{  
  // Start rendering loop
  while (m_Window->isOpen()) {

    // Process events
    sf::Event Event;
    while (m_Window->pollEvent(Event)) {

      // Close window : exit
      if (Event.type == sf::Event::Closed) {
	m_Window->close();
      }

      // Escape key : exit
      if ((Event.type == sf::Event::KeyPressed) 
	  && (Event.key.code == sf::Keyboard::Escape)) {
	m_Window->close();
      }

      // Resize event : adjust viewport
      if (Event.type == sf::Event::Resized) {
	glViewport(0, 0, Event.size.width, Event.size.height);
      }

      // Check for W, A, S, D keys...
      if ((Event.type == sf::Event::KeyPressed) 
	  && (Event.key.code == sf::Keyboard::W)) {
	// Move your camera forward
      }

      if (Event.type == sf::Event::MouseButtonPressed) {
	if (Event.mouseButton.button == sf::Mouse::Left) {
	  // Do something with the mouse data
	}
      }
    }

    // Set the active window before using OpenGL commands. For a
    // single window OpenGL program this really doesn't do much, but
    // if you did have multiple windows, you might need to "render"
    // different things to each window, and thus, you need to make
    // each "window app" active before issuing OpenGL commands.
    m_Window->setActive();

    // Place your OpenGL calls here that allow the screen to be
    // refreshed each time this loop is executed.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();    

    glEnableClientState(GL_VERTEX_ARRAY); 
    glEnableClientState(GL_COLOR_ARRAY); 
    glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
    glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO);

    glVertexPointer(3, GL_FLOAT, 8 * sizeof(float), 0);
    glColorPointer(3, GL_FLOAT, 8 * sizeof(float), (GLvoid*)(3*sizeof(float)));
    glTexCoordPointer(2, GL_FLOAT, 8 * sizeof(float), (GLvoid*)(6*sizeof(float)));

    if (m_useShaders) {
      simpleShader.activate();
    }

    glDrawArrays(GL_TRIANGLES, 0, m_numVertices);

    if (m_useShaders) {
      simpleShader.deactivate();
    }

    glDisableClientState(GL_VERTEX_ARRAY);    
    glDisableClientState(GL_COLOR_ARRAY);    
    glDisableClientState(GL_TEXTURE_COORD_ARRAY); 
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Finally, display rendered frame on screen - this is the command
    // that tells OpenGL to actually refresh the window you're looking
    // at with what you wrote to memory in the commands above.
    m_Window->display();
  }
}  
