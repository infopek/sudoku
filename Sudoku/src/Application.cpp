#include <iostream>

#include <SFML/Graphics.hpp>

#include "Board.h"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Sudoku", sf::Style::Close);

    sf::Texture texture;
    texture.loadFromFile("background_gray.jpg");
    sf::Sprite background(texture);
    background.setColor(sf::Color(170, 170, 170));

    const sf::Vector2f cellSize(50.0f, 50.0f);
    const int n = 9;
    const unsigned int fps = 60;

    Board board(n, sf::Vector2f(50.0f, 50.0f));
    window.setFramerateLimit(fps);
    while (window.isOpen())
    {
        // Event handling
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
            }
        }

        window.clear(sf::Color(42, 42, 42, 1)); // dark grey

        // Draw calls
        window.draw(background);
        board.Draw(window);

        window.display();
    }

}