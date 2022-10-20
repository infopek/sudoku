#include <iostream>

#include <SFML/Graphics.hpp>

#define WIN_WIDTH 1080
#define WIN_HEIGHT 720

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Sudoku", sf::Style::Close | sf::Style::Resize);
    sf::Color darkGrey = sf::Color(42, 42, 42, 1);

    while (window.isOpen())
    {
        // Event listener
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                // Resize
                break;
            default:
                break;
            }
        }

        window.clear(darkGrey);

        // Draw calls

        window.display();
    }

}