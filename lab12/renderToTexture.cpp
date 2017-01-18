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
#include <SFML/Graphics.hpp>

#include "handleGraphicsArgs.h"

class OpenGLRenderWindow {
public:
    OpenGLRenderWindow();
    OpenGLRenderWindow(const int winWidth, const int winHeight);
    ~OpenGLRenderWindow();

    void initializeOpenGLState();
    void generateVBOQuads();
    void run();

private:
    void instanceSFMLState();

    int counter;
    int m_winWidth, m_winHeight;
    int m_texWidth, m_texHeight;

    sf::RenderWindow *m_App;
//    sf::Font sfmlFont;
//    sf::String sfmlTextString;

    GLuint texID[1];
    GLuint renderToTextureQUAD_VBO, displayQUAD_VBO;

    // Create a new framebuffer object and bind it so that we can use it
    // and set it up as we need
    GLuint m_fbo;
};

int main(int argc, char *argv[]) {
    sivelab::GraphicsArgs args;
    args.process(argc, argv);

    OpenGLRenderWindow oglWin(args.width, args.height);
    oglWin.initializeOpenGLState();
    oglWin.run();

    exit(EXIT_SUCCESS);
}

OpenGLRenderWindow::OpenGLRenderWindow()
  : counter(0),
    m_winWidth(100), m_winHeight(100),
    m_texWidth(500), m_texHeight(500)
{
  instanceSFMLState();
}

OpenGLRenderWindow::OpenGLRenderWindow(const int winWidth, const int winHeight)
  : counter(0),
    m_winWidth( winWidth ), m_winHeight( winHeight ),
    m_texWidth(500), m_texHeight(500)
{
  instanceSFMLState();
}

OpenGLRenderWindow::~OpenGLRenderWindow() {
}

void OpenGLRenderWindow::instanceSFMLState() {
  // Create the main window using the width and height parameters from
  // our command line arguments.
  m_App = new sf::RenderWindow(sf::VideoMode(m_winWidth, m_winHeight, 32), "SFML OpenGL RenderToTexture");

  // To load your own font, it would be like this:
  // sfmlFont.LoadFromFile( "nevis.ttf" );
  // sfmlTextString.SetFont( sfmlFont );

  // But for now, use the default font
//  sfmlTextString.SetFont(sf::Font::GetDefaultFont());
}

void OpenGLRenderWindow::initializeOpenGLState() {
  glewInit();
  //
  // Setup the orthographic projection as it is a good default for this example.
  //
  glViewport(0, 0, m_winWidth, m_winHeight);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1, 1, -1, 1, -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // We will be using non-power of two textures, so just enable it once
  glEnable(GL_TEXTURE_RECTANGLE);

  // Generate the VBOs we'll need in this example.
  generateVBOQuads();

  // ///////////////////////////////////////////////////////
  // TEXTURE INITIALIZATION
  //
  // We will need one texture in which to use as the output buffer for
  // our FrameBufferObject
  //
  glGenTextures(1, texID);
  int textureSize = m_texWidth * m_texHeight * 3;
  float *textureData = new float[ textureSize ];
  memset(textureData, 1, textureSize);

  //
  // Primary texture output for the FBO
  //
  glBindTexture(GL_TEXTURE_RECTANGLE, texID[0]);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB,
          m_texWidth, m_texHeight,
          0, GL_RGB, GL_FLOAT, textureData);
  glBindTexture(GL_TEXTURE_RECTANGLE, 0);

  delete [] textureData;

  // ///////////////////////////////////////////////////////
  // FRAME BUFFER INITIALIZATION
  //
  // Will support the Render to texture
  //

  // Create a new framebuffer object and bind it so that we can use it
  // and set it up as we need
  glGenFramebuffers(1, &m_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

  // By attaching a texture to the FBO, we can write to that
  // texture. More than one FBO attachment can be written in a GLSL
  // shader, but in this example we will only make an attachment
  // between the Framebuffer's COLOR_ATTACHMENT0 and our texID[0]
  // allocated above.
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, texID[0], 0);

  // Good to check if the frame buffer is OK at this point -- here's how
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE)  // If the frame buffer does not report back as complete
    {
      std::cout << "Couldn't create frame buffer" << std::endl; // Make sure you include <iostream>
      exit(0); // Exit the application
    }

  // Disable the frame buffer just sets the GL state back to default
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // You can also check OpenGL error states with code like
  // this... note that you really need to call it after each OpenGL
  // function to know if an error occurred at a specific point in your
  // code.
  GLenum errCode;
  if ((errCode = glGetError()) != GL_NO_ERROR) {
      std::cout << "OpenGL Error: " << gluErrorString(errCode) << std::endl;
    }
}

void OpenGLRenderWindow::generateVBOQuads() {
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  // //////////////////////////////////////////////
  // Generate the QUAD for render to texture
  // //////////////////////////////////////////////
  glGenBuffers(1, &renderToTextureQUAD_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, renderToTextureQUAD_VBO);

  // 3 vertices each with 3 floats (X, Y, Z) per vertex followed by 3 colors and 2 texture coordinates floats
  int tIdx = 0;
  int numFloats = 4 * (3 + 3 + 2);
  float* host_VertexBuffer = new float[ numFloats ];

  host_VertexBuffer[ tIdx++ ] = -1.0f;   // vertex
  host_VertexBuffer[ tIdx++ ] = -1.0f;
  host_VertexBuffer[ tIdx++ ] = 0.0f;
  host_VertexBuffer[ tIdx++ ] = 1.0f;   // colors
  host_VertexBuffer[ tIdx++ ] = 0.0f;
  host_VertexBuffer[ tIdx++ ] = 0.0f;
  host_VertexBuffer[ tIdx++ ] = 0.0f;    // tex coord
  host_VertexBuffer[ tIdx++ ] = 0.0f;

  host_VertexBuffer[ tIdx++ ] = 1.0f;   // vertex
  host_VertexBuffer[ tIdx++ ] = -1.0f;
  host_VertexBuffer[ tIdx++ ] = 0.0f;
  host_VertexBuffer[ tIdx++ ] = 0.0f;   // colors
  host_VertexBuffer[ tIdx++ ] = 1.0f;
  host_VertexBuffer[ tIdx++ ] = 1.0f;
  host_VertexBuffer[ tIdx++ ] = m_texWidth;    // tex coord
  host_VertexBuffer[ tIdx++ ] = 0.0f;

  host_VertexBuffer[ tIdx++ ] = 0.0f;   // vertex
  host_VertexBuffer[ tIdx++ ] = 1.0f;
  host_VertexBuffer[ tIdx++ ] = 0.0f;
  host_VertexBuffer[ tIdx++ ] = 0.0f;   // colors
  host_VertexBuffer[ tIdx++ ] = 0.0f;
  host_VertexBuffer[ tIdx++ ] = 1.0f;
  host_VertexBuffer[ tIdx++ ] = m_texWidth;    // tex coord
  host_VertexBuffer[ tIdx++ ] = m_texHeight;

  host_VertexBuffer[ tIdx++ ] = 0.0f;   // vertex
  host_VertexBuffer[ tIdx++ ] = 1.0f;
  host_VertexBuffer[ tIdx++ ] = 0.0f;
  host_VertexBuffer[ tIdx++ ] = 1.0f;   // colors
  host_VertexBuffer[ tIdx++ ] = 1.0f;
  host_VertexBuffer[ tIdx++ ] = 0.0f;
  host_VertexBuffer[ tIdx++ ] = m_texWidth;    // tex coord
  host_VertexBuffer[ tIdx++ ] = m_texHeight;

  glBufferData(GL_ARRAY_BUFFER, numFloats * sizeof(GLfloat), host_VertexBuffer, GL_STATIC_DRAW);

  int stride = 8 * sizeof(float);
  glVertexPointer(3, GL_FLOAT, stride, 0);
  glColorPointer(3, GL_FLOAT, stride, (GLvoid*)(3*sizeof(float)));
  glTexCoordPointer(2, GL_FLOAT, stride, (GLvoid*)(6*sizeof(float)));

  delete [] host_VertexBuffer;
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glDisableClientState(GL_COLOR_ARRAY);

  // //////////////////////////////////////////////
  // Generate the QUAD for final display
  // //////////////////////////////////////////////
  glGenBuffers(1, &displayQUAD_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, displayQUAD_VBO);

  // 4 vertices each with 3 floats (X, Y, Z) per vertex followed by 2 texture coordinates floats
  tIdx = 0;
  host_VertexBuffer = new float[ numFloats ];

  host_VertexBuffer[ tIdx++ ] = -0.98f;   // vertex
  host_VertexBuffer[ tIdx++ ] = -0.98f;
  host_VertexBuffer[ tIdx++ ] = 0.0f;
  host_VertexBuffer[ tIdx++ ] = 0.0f;    // tex coord
  host_VertexBuffer[ tIdx++ ] = 0.0f;

  host_VertexBuffer[ tIdx++ ] = 0.98f;   // vertex
  host_VertexBuffer[ tIdx++ ] = -0.98f;
  host_VertexBuffer[ tIdx++ ] = 0.0f;
  host_VertexBuffer[ tIdx++ ] = m_texWidth;    // tex coord
  host_VertexBuffer[ tIdx++ ] = 0.0f;

  host_VertexBuffer[ tIdx++ ] = 0.98f;   // vertex
  host_VertexBuffer[ tIdx++ ] = 0.98f;
  host_VertexBuffer[ tIdx++ ] = 0.0f;
  host_VertexBuffer[ tIdx++ ] = m_texWidth;    // tex coord
  host_VertexBuffer[ tIdx++ ] = m_texHeight;

  host_VertexBuffer[ tIdx++ ] = -0.98f;   // vertex
  host_VertexBuffer[ tIdx++ ] = 0.98f;
  host_VertexBuffer[ tIdx++ ] = 0.0f;
  host_VertexBuffer[ tIdx++ ] = 0.0f;    // tex coord
  host_VertexBuffer[ tIdx++ ] = m_texHeight;

  glBufferData(GL_ARRAY_BUFFER, numFloats * sizeof(GLfloat), host_VertexBuffer, GL_STATIC_DRAW);

  glVertexPointer(3, GL_FLOAT, 5 * sizeof(float), 0);
  glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(float), (GLvoid*)(3*sizeof(float)));

  delete [] host_VertexBuffer;
  glBindBuffer(GL_ARRAY_BUFFER, 0);

}


void OpenGLRenderWindow::run() {
    // Start rendering loop
    while (m_App->isOpen()) {
        // Process events
        sf::Event Event;
        while (m_App->pollEvent(Event)) {
            // Close window : exit
            if (Event.type == sf::Event::Closed)
                m_App->close();

            // Escape key : exit
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
                m_App->close();
        }

        // Set the active window before using OpenGL commands
        // It's useless here because active window is always the same,
        // but don't forget it if you use multiple windows or controls
        m_App->setActive();

        // If you want to use SFML's abilities to place font-based
        // text on the screen, here's an example:
//        sfmlTextString.setText("Render To Texture!");
//        sfmlTextString.setSize(50);
//        sfmlTextString.setColor(sf::Color::Red);

        // //////////////////////////////////////////////////////////
        //
        // PASS 1 - Any OpenGL output (like your scene) is rendered to
        // the FBO, and hence, texID[0]
        //
        // TO do this, we must first bind the FBO to which we will
        // write, and set up the rendering parameters as necessary.
        //
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo); {
            // The viewport size here is the texture size that the
            // framebuffer is attached to... NOT your window size!
            glViewport(0, 0, m_texWidth, m_texHeight);

            // When you render to a texture, you may have different
            // viewing and projection settings too
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(80.0, 1, 1, 100);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            // Do normal OpenGL Stuff here...

            // Green background rendered in this scene
            glClearColor(0.0, 1.0, 0.0, 1.0);

            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //
            // Your OpenGL Code for rendering various scene or what-not
            // can go here!

            // For instance, we'll render a rectangle

            glPushMatrix();
                glTranslatef(1, 1, -5);

                // Don't want this to be textured in this example
                glBindTexture(GL_TEXTURE_RECTANGLE, 0);

                glBindBuffer(GL_ARRAY_BUFFER, renderToTextureQUAD_VBO);
                glEnableClientState(GL_COLOR_ARRAY);
                glDrawArrays(GL_QUADS, 0, 4);
                glDisableClientState(GL_COLOR_ARRAY);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            glPopMatrix();
        }
        // Unbind the framebuffer -- we are done rendering to texture
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        // ///////////////////////////////////
        //
        // FINAL RENDERING PASS - now that we've prepared the texture
        // in the previous step, let's rendering the actual scene!
        //

        // We must set the viewport and scene parameters back to what
        // we want them to be in order for this to work correctly
        glViewport(0, 0, m_winWidth, m_winHeight);

        // Also make sure you set the viewing and projection matrices
        // back to what you want for the user's view
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, 1, 1, 100);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Now, we want a grey background for our scene
        glClearColor( 0.5, 0.5, 0.5, 1.0 );

        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_RECTANGLE, texID[0]);

        glPushMatrix();

        // The displayQUAD_VBO is a 2 x 2 QUAD in X and Y, placed at
        // the Z=0 plane, so translate it back a bit.
        glTranslatef(0, 0, -10);

        glBindBuffer(GL_ARRAY_BUFFER, displayQUAD_VBO);
        glDrawArrays(GL_QUADS, 0, 4);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glPopMatrix();

        m_App->display();
    }
}

