#include <GL/glew.h>
#include <SFML/Window.hpp>

#include "handleGraphicsArgs.h"
#include "Vector3D.h"
#include "png++/png.hpp"

#include "framebufferObject.h"
#include "GLSL.h"

using namespace sivelab;

// 1) Code reads in an image into a texture
//
// 2) GLSL renders that texture into an FBO making some color changes
// to it (for example, rendering stripes of the image as negatives of
// the original color, leaving adjacent stripes alone) -- preferably
// using an early version of GLSL like #version 110 or #version 120
// since my laptop can't seem to render the newer stuff.
//
// 3) displaying that FBO on the screen

int main(int argc, char *argv[])
{
  // Process the command line arguments
  GraphicsArgs args;
  args.process(argc, argv);

  // Create the main window using the width and height parameters from
  // our command line arguments.
  sf::Window sfmlWindow(sf::VideoMode(args.width, args.height, 32), "SFML OpenGL");

  // Create a clock for measuring time elapsed - not used in this example though
  sf::Clock Clock;

  // Use GLEW to initialize the GL state - what this mostly means is
  // that the bindings between function calls and their locations in
  // the driver code will be correctly established. This is simpler
  // and easier to deal with over using the traditional GLX extension
  // stuff.
  // 
  // Initialize the GL system using GLEW 
  glewInit();

  // Set color to ensure we get the state set at some point.
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

  // Disable Z-buffer read and write - just pixel blitting
  glDisable(GL_DEPTH_TEST);

  // 
  // Setup the orthographic projection since we just want to see the
  // textured quad with our PNG image - making sure its set now in the
  // setup.
  // 
  glViewport(0, 0, args.width, args.height);
  
  // To manipulate the "projection" we need to switch the GL matrix to
  // the PROJECTION matrix. Any calls that manipulate matrices will
  // then affect the projection matrix.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1, 1, -1, 1, -1, 1);

  // Now, switch back to the MODELVIEW matrix so we don't
  // inadvertantly mess up the projection.
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // /////////////////////////////////////////////// 
  // Create our texture memory and/or Load textures from images so we
  // can use it in the shader.
  //
  GLuint texIDArray[3];
  glGenTextures(3, texIDArray);

  // All textures are sized to be the window width and height
  int textureSize = args.width * args.height * 3;

  // *** First texture - load up an image from the -i argument from
  // *** the command line:
  std::cout << "Reading texture map data from file: " << args.inputFileName << std::endl;
  png::image< png::rgb_pixel > texPNGImage;
  texPNGImage.read( args.inputFileName );

  int pngWidth = texPNGImage.get_width();
  int pngHeight = texPNGImage.get_height();
  float *copiedPNGData = new float[ pngHeight * pngWidth * 3 ];

  int tIdx = 0;
  for (unsigned int idx=0; idx<pngHeight*pngWidth; ++idx) {
    size_t col = idx % pngWidth;
    size_t row = static_cast<size_t>( floor(idx / static_cast<float>(pngWidth)) );
      
    png::rgb_pixel texel = texPNGImage[ pngHeight-1 - row ][col];
    
    copiedPNGData[tIdx++] = texel.red/255.0;
    copiedPNGData[tIdx++] = texel.green/255.0;
    copiedPNGData[tIdx++] = texel.blue/255.0;
  }
  
  glBindTexture(GL_TEXTURE_RECTANGLE, texIDArray[0]);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB, 
	       pngWidth, pngHeight, 
	       0, GL_RGB, GL_FLOAT, copiedPNGData);
  glBindTexture(GL_TEXTURE_RECTANGLE, 0);
  delete [] copiedPNGData;

  // Create two additional textures, one with random data, and one to
  // use as the result, or output for the FBO.
  float *texData = new float[ pngHeight * pngWidth * 3 ];

  tIdx = 0;
  for (unsigned int idx=0; idx<pngHeight*pngWidth; ++idx) {
    texData[tIdx++] = drand48();
    texData[tIdx++] = drand48();
    texData[tIdx++] = drand48();
  }
  
  glBindTexture(GL_TEXTURE_RECTANGLE, texIDArray[1]);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB, 
	       pngWidth, pngHeight, 
	       0, GL_RGB, GL_FLOAT, texData);
  glBindTexture(GL_TEXTURE_RECTANGLE, 0);
  delete [] texData;
  
  // The third texture is used for output, so we just make space for
  // it on the graphics card with these steps. This is sized to be the
  // window size... not the pngwidth just so you know.
  glBindTexture(GL_TEXTURE_RECTANGLE, texIDArray[2]);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB, 
	       args.width, args.height, 
	       0, GL_RGB, GL_FLOAT, 0);
  glBindTexture(GL_TEXTURE_RECTANGLE, 0);
  
    
  // ////////////////////////////////////////////////////////////
  // Framebuffer object
  //
  // In this example, we'll render to a framebuffer object using
  // shaders. Then the contents of the framebuffer will be copied back
  // out to the host, written to a PNG image, and then exit.
  //
    
  // Query the graphics card to determine the max number of
  // attachments for FBOs.
  GLint maxColorAttachments = 0;
  glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, (GLint*)&maxColorAttachments);
  std::cout << "OpenGL State: GL_MAX_COLOR_ATTACHMENTS = " << maxColorAttachments << std::endl;

  // Create a new framebuffer object and bind it so that we can use it
  // and set it up as we need
  FramebufferObject *fbo = new FramebufferObject();
  fbo->Bind();
  
  // By attaching a texture to the FBO, we can write to that
  // texture. More than one FBO attachment can be written in a GLSL
  // shader, but in this example we will only write to the 3rd texture
  // allocated above.
  fbo->AttachTexture(GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_RECTANGLE, texIDArray[2]);

  // Just verify that things are OK, so watch for the error!
  if (fbo->IsValid() == false) std::cout << "\terror with fbo FrameBuffer." << std::endl;

  // Disable the frame buffer just sets the GL state back to default
  FramebufferObject::Disable();

  // /////////////////////////////////////////////////////////////
  // Setup the GLSL shader(s) that will do the computations
  // here. These are loaded from text files and compiled onto the
  // graphics card.
  GLSLObject computeShader;
  computeShader.addShader("vp_default.glsl", GLSLObject::VERTEX_SHADER);
  computeShader.addShader("fp_passthrough.glsl", GLSLObject::FRAGMENT_SHADER);
  computeShader.createProgram();

  computeShader.activate();
  // We have to set up the uniform variables that link with the
  // variables in the shader. uniform variables allow communication
  // between the host and device. Shader must be "activated" or bound
  // to perform these steps.
  GLuint uniform_cs_texId0 = computeShader.createUniform("texId0");

  GLuint uniform_cs_texId1 = computeShader.createUniform("texId1");
  computeShader.deactivate();

#if 0
  while (sfmlWindow.isOpen())
    {
      // Process events
      sf::Event Event;
      while (sfmlWindow.pollEvent(Event))
        {
	  // Close window : exit
	  if (Event.type == sf::Event::Closed)
	    sfmlWindow.close();

	  // Escape key : exit
	  if ((Event.type == sf::Event::KeyPressed) 
	      && (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))) {
	    sfmlWindow.close();
	  }
        }
#endif

      // Set the active window before using OpenGL commands
      // It's useless here because active window is always the same,
      // but don't forget it if you use multiple windows or controls
      sfmlWindow.setActive();

      // 
      // Place your OpenGL draw code here!  This is inside the
      // "event loop" and these calls will happen over and over
      // until the app is terminated.
      // 
      // The first thing we want to do is clear the color and depth buffers.
      // Clear color and depth buffer
      glClear(GL_COLOR_BUFFER_BIT);
      glLoadIdentity();    

      glEnable(GL_TEXTURE_RECTANGLE);

      // Now, you can draw your own OpenGL between here...
      // ---------------------- start OpenGL rendering here -------------------

      // 1) Bind the FBO to which we will write
      fbo->Bind();

      glClear(GL_COLOR_BUFFER_BIT);

      computeShader.activate();
  
      // Bind the textures correctly and pass in the texture unit
      // identifiers to the shader
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_RECTANGLE, texIDArray[0]);
      glUniform1iARB(uniform_cs_texId0, 0);

      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_RECTANGLE, texIDArray[1]);
      glUniform1iARB(uniform_cs_texId1, 1);

      glColor3f(1,0,1);
      glBegin(GL_QUADS);
      {
	glTexCoord2f(0, 0);                 glVertex3f(-0.95, -0.95, 0.0f);
	glTexCoord2f(pngWidth, 0);          glVertex3f( 0.95, -0.95, 0.0f);
	glTexCoord2f(pngWidth, pngHeight);  glVertex3f( 0.95,  0.95, 0.0f);
	glTexCoord2f(0, pngHeight);         glVertex3f(-0.95, 0.95, 0.0f);
      }
      glEnd();

      computeShader.deactivate();

      // Lastly, copy the texture out of the fbo and write to a PNG
      float *fboData = new float[ args.width * args.height * 3 ];

      // Set the read buffer
      glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
      glReadPixels(0, 0, args.width, args.height, GL_RGB, GL_FLOAT, fboData);
  
      std::string oFilename;
      if (!args.isSet("outputfile", oFilename)) 
	oFilename = "fboOutput.png";

      png::image< png::rgb_pixel > outputData(args.width, args.height);
      for (int idx=0; idx<args.width*args.height; idx++) {
	
	int x = (idx) % args.width;  	    // which row
	int y = (int)floor(idx / (float)(args.width));  // which column
	
	// From Author of PNG++: Actually, you should want image[Y][X]
	// since first [] gets you to Y-th row, and then to the X-th
	// column in that row.
	outputData[args.height - y - 1][x] = png::rgb_pixel( 255 * fboData[idx * 3], 
					   255 * fboData[idx * 3 + 1], 
					   255 * fboData[idx * 3 + 2]);
      }
      outputData.write( oFilename );
      
      FramebufferObject::Disable();

      // ----------------------  end OpenGL rendering here -------------------
      
#if 0
      // Finally, display rendered frame on screen
      sfmlWindow.display();
      done = true;
    }
#endif

  sfmlWindow.close();

  return EXIT_SUCCESS;
}
