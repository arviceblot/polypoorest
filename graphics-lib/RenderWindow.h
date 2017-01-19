#pragma once

#include <GL/glew.h>
#include <SFML/Graphics.hpp>

class RenderWindow
{
public:
    RenderWindow(float *image, const int width = 100, const int height = 100);
    ~RenderWindow();

    void run();

private:
    int counter;
    int width, height;
    float *image;
    std::uint8_t *pixels;

    sf::RenderWindow * window;
    sf::Texture texture;
    sf::Sprite sprite;
};
