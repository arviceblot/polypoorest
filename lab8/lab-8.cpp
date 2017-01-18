/**
 * lab-8.cpp
 */

#include <cstdlib>
#include <iostream>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include "GLSL.h"
#include "Vector3D.h"
#include "VBO.h"

using namespace sivelab;

int main(int argc, char* argv[]) {
    sf::Window* window = new sf::Window(sf::VideoMode(500, 500, 32), "SFML OpenGL");
    GLuint triangleVBO;

    GLSLObject simpleShader;

    glewInit();

    window->setActive();

    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_RECTANGLE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glViewport(0, 0, 500, 500);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60.0, 1, 1.0, 500);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(3, 3, 5, 0, 0, 0, 0, 1, 0);

    glFrontFace(GL_CCW);

    glGenBuffers(1, &triangleVBO);

    glMatrixMode(GL_MODELVIEW);

    // ///////////////////////////////////////////////
    // Create our texture memory and/or Load textures from images so we
    // can use it in the shader.
    //
    GLuint texIDArray[2];
    glGenTextures(2, texIDArray);

    // All textures are sized to be the window width and height
    int textureSize = 500 * 500 * 2;

    // Create two additional textures, one with random data, and one to
    // use as the result, or output for the FBO.
    float* texData = new float[500 * 500 * 3];

    int tIdx = 0;
    for (unsigned int idx = 0; idx < 500 * 500; ++idx) {
        texData[tIdx++] = drand48();
        texData[tIdx++] = drand48();
        texData[tIdx++] = drand48();
    }

    glBindTexture(GL_TEXTURE_RECTANGLE, texIDArray[0]);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB,
	             500, 500,
	             0, GL_RGB, GL_FLOAT, texData);
    glBindTexture(GL_TEXTURE_RECTANGLE, 0);
    delete [] texData;

    // The third texture is used for output, so we just make space for
    // it on the graphics card with these steps. This is sized to be the
    // window size... not the pngwidth just so you know.
    glBindTexture(GL_TEXTURE_RECTANGLE, texIDArray[1]);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB,
	             500, 500,
	             0, GL_RGB, GL_FLOAT, 0);
    glBindTexture(GL_TEXTURE_RECTANGLE, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_RECTANGLE, texIDArray[0]);

    // Query the graphics card to determine the max number of
    // attachments for FBOs.
    GLint maxColorAttachments = 0;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, (GLint*)&maxColorAttachments);
    std::cout << "OpenGL State: GL_MAX_COLOR_ATTACHMENTS = " << maxColorAttachments << std::endl;

    // lighting info
    GLfloat diffuseLightVec[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ambientLightVec[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightPosition[] = {0.0f, 10.0f, 0.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLightVec);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, ambientLightVec);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    VBO box(triangleVBO);
    VBO floor(triangleVBO, Vector3D(0.5f, 0.5f, 0.5f), Vector3D(-5.0f, -1.0f, -5.0f), Vector3D(4.0f, -0.5f, 4.0f));

    simpleShader.addShader("vertexShader.glsl", GLSLObject::VERTEX_SHADER);
    simpleShader.addShader("fragmentShader.glsl", GLSLObject::FRAGMENT_SHADER);
    simpleShader.createProgram();

    sf::Event event;
    while (window->isOpen()) {
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window->close();
                }
                if (event.key.code == sf::Keyboard::W) {
                    lightPosition[2] -= 1.0f;
                    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
                }
                if (event.key.code == sf::Keyboard::A) {
                    lightPosition[0] -= 1.0f;
                    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
                }
                if (event.key.code == sf::Keyboard::S) {
                    lightPosition[2] += 1.0f;
                    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
                }
                if (event.key.code == sf::Keyboard::D) {
                    lightPosition[0] += 1.0f;
                    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
                }
            }
        }

        window->setActive();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        gluLookAt(3, 3, 3, 0, 0, 0, 0, 1, 0);

        floor.load();
        simpleShader.activate();
        floor.render();
        simpleShader.deactivate();

        box.load();
        box.render();

        window->display();
    }
}
