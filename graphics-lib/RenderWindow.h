#pragma once

#include <string>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>

class RenderWindow
{
public:
    RenderWindow(const std::string &windowName, float *image, const int width = 100, const int height = 100);
    ~RenderWindow();

    void operator()();

private:
    int width, height;
    float *image;
    std::uint8_t *pixels;

    sf::RenderWindow *window;
    sf::Texture texture;
    sf::Sprite sprite;
};

inline RenderWindow::~RenderWindow() {}
