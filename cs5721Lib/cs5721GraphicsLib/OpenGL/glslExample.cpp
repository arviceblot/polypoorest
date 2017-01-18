//
// compilation instructions
//
// OS X
// 
// g++ -Wall -O3 -o loadOBJ loadOBJ.cpp glm.c -I. -framework GLUT -framework OpenGL -framework Foundation

//
// Win32
//
// 1. Create project (Visual C++/Win32/Win32 Console).
// 2. Paste all files in this directory into the project directory.
// 3. Compile & Run.  It works!
//

//
// Linux
//
// g++ -Wall -O3 -o loadOBJ loadOBJ.cpp glm.c -I. -lglut -lGL -lGLU
//

#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <cstdio>

#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#ifdef WIN32
#include <glut.h>
#else
#include <GL/glut.h>
#endif
#endif

#include "GLSL.h"
#include "glm.h"

using namespace std;

void initialize_OpenGLState(void);
void text(int x, int y, char* s);

void idle(void);
void display(void);
void resizeWindow(int w, int h);
void keyboard(unsigned char key,int x,int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);

// GLUT Window variables
static int window_id;
static int win_width = 800, win_height = (int)(win_width * 9.0/16.0);

// Eyepoint variables
static float field_of_view = 45.0;
static float eye_pos[3];
static float eye_gaze[3];

// Rotation and Translation variables
static bool rotate_object = false;
static GLfloat azimuth = 0.0;
static GLfloat elevation = 0.0;

GLSLObject shader_object;
GLuint texid[10];
GLint uniform_texunit0;

int main( int argc, char *argv[] )
{
    // arguments to main are width and height of window
    if (argc == 3) 
    {
	win_width = atoi( argv[1] );
	win_height = atoi( argv[2] );
    }

    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );

    glutInit(&argc, argv);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(win_width, win_height);
    window_id = glutCreateWindow(argv[0]);

    GLenum err = glewInit();
    if (GLEW_OK != err) 
      {
	std::cout << "Error: " << glewGetErrorString(err) << std::endl;
      }
    else 
      {
	std::cout << "GLEW Initialized OK." << std::endl;
      }
    
    if (GL_ARB_vertex_buffer_object) 
      {
	std::cout << "GL_ARB_vertex_buffer_object available!" << std::endl;
      }
    else 
      {
	std::cout << "GL_ARB_vertex_buffer_object is NOT available!  Exiting!" << std::endl;
	exit(-1);
      }
    
    glutDisplayFunc(display);
    glutReshapeFunc(resizeWindow);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);

    initialize_OpenGLState();

    glutMainLoop();
    return 0;
}

void idle(void)
{
    glutPostRedisplay();
}

void resizeWindow(int width, int height) 
{
    float ratio = (float)width/(float)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( field_of_view, ratio, 1, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt( eye_pos[0], eye_pos[1], eye_pos[2],
	       eye_gaze[0], eye_gaze[1], eye_gaze[2],
	       0, 1, 0 );
}

void initialize_OpenGLState(void)
{
    eye_pos[0] = 0;
    eye_pos[1] = 0;
    eye_pos[2] = 0;

    eye_gaze[0] = 0;
    eye_gaze[1] = 0;
    eye_gaze[2] = -5;

    glEnable(GL_DEPTH_TEST);

    // load shader
    shader_object.addShader("vertex_program_01.glsl", GLSLObject::VERTEX_SHADER);
    shader_object.addShader("fragment_program_01.glsl", GLSLObject::FRAGMENT_SHADER);
    shader_object.createProgram();
}

void display(void)
{
    static char text_buffer[128];
    static bool do_once = true;
    static bool frame_rate = true;
    static double avg_frame_rate = 0.0;
    static struct timeval startframe, endframe, diff;

    if (do_once)
    {
	gettimeofday(&startframe, 0); 
	do_once = false;
    }

    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // change mouse motions to modify field of view and eye position
    gluLookAt( eye_pos[0], eye_pos[1], eye_pos[2],
	       eye_gaze[0], eye_gaze[1], eye_gaze[2],
	       0, 1, 0 );

    glPushMatrix();
    glTranslatef(0,0,-5.0);
    glRotatef(elevation, 1,0,0);
    glRotatef(azimuth, 0,1,0);

    shader_object.activate();

    glColor3f(1.0,0.0,0.0);
    // glutSolidCube(1.0);
    glutSolidTeapot(1.0);

    shader_object.deactivate();

    glPopMatrix();
      
    // spit out frame rate.
    if (frame_rate) 
    {
	gettimeofday(&endframe, 0); 
	timersub(&endframe, &startframe, &diff);
	avg_frame_rate = 1.0 / (diff.tv_sec + diff.tv_usec/1.0e6);

	sprintf(text_buffer, "%.2f fps", avg_frame_rate);

	text(5, 5, text_buffer);
	gettimeofday(&startframe, 0); 
    }
  
    glutSwapBuffers();
}

void keyboard(unsigned char key,int x,int y)
{
    if (key=='s') 
    {
	rotate_object = !rotate_object;
    }
    
    else if (key==27)  // the escape key
    {
	glutDestroyWindow(window_id);
	exit(0);
    }
}

static int last_x, last_y;
void mouse(int button, int state, int x, int y)
{
    last_x = x;
    last_y = y;

    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	rotate_object = true;
    else // state == GLUT_UP
	rotate_object = false;

    glutPostRedisplay();
}

void motion(int x, int y)
{
    if (rotate_object) 
    {
	// since y is up, move eye in z only to take it into and out of the screen
	float change = x - last_x;
	float rate = 0.1;

	change = x - last_x;
	rate = 0.1;
	azimuth = azimuth + change * rate;

	change = y - last_y;
	rate = 0.1;
	elevation = elevation + change * rate;
    }

    last_x = x;
    last_y = y;

    glutPostRedisplay();
}

/* text: draws a string of text with an 18 point helvetica bitmap font
   at position (x,y) in window space (bottom left corner is (0,0). */
void text(int x, int y, char* s) 
{
    int lines;
    char* p;

    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, glutGet(GLUT_WINDOW_WIDTH), 
	    0, glutGet(GLUT_WINDOW_HEIGHT), -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3ub(0, 0, 0);
    glRasterPos2i(x+1, y-1);
    for (p=s, lines=0; *p; p++) {
	if (*p == '\n') {
	    lines++;
	    glRasterPos2i(x+1, y-1-(lines*18));
	}
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
    }
    glColor3ub(255, 255, 0);
    glRasterPos2i(x, y);
    for (p=s, lines=0; *p; p++) {
	if (*p == '\n') {
	    lines++;
	    glRasterPos2i(x, y-(lines*18));
	}
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
    }
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
}
