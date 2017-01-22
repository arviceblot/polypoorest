#include "RenderWindow.h"

RenderWindow::RenderWindow(const std::string &windowName, float *image, const int width, const int height)
    : width(width), height(height), image(image)
{
    // Create the main window using the width and height parameters from
    // our command line arguments.
    window = new sf::RenderWindow(sf::VideoMode(width, height, 32), windowName);
    window->setFramerateLimit(60);

    texture.create(width, height);
    sprite = sf::Sprite(texture);
    pixels = new std::uint8_t[4 * width * height];
}

void RenderWindow::operator()()
{
    // Start rendering loop
    while (window->isOpen())
    {
        // Process events
        sf::Event Event;
        while (window->pollEvent(Event))
        {
            // Close window : exit
            if (Event.type == sf::Event::Closed)
            {
                window->close();
            }

            // Escape key : exit
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
            {
                window->close();
            }
        }

        // Set the active window before using OpenGL commands
        // It's useless here because active window is always the same,
        // but don't forget it if you use multiple windows or controls
        //window->setActive();

        window->clear();

        // update pixels from thread data image
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                pixels[(x + y * width) * 4] = static_cast<uint8_t>(std::floor(image[(x + y * width) * 3] * 255));
                pixels[1 + (x + y * width) * 4] = static_cast<uint8_t>(std::floor(image[1 + (x + y * width) * 3] * 255));
                pixels[2 + (x + y * width) * 4] = static_cast<uint8_t>(std::floor(image[2 + (x + y * width) * 3] * 255));
                pixels[3 + (x + y * width) * 4] = 0xFF;
            }
        }

        texture.update(pixels);
        window->draw(sprite);
        window->display();
    }
}
