#include "Game.h"

Game::Game(unsigned int winWidth, unsigned int winHeight)
    : m_Fps(60), m_Window(sf::RenderWindow(sf::VideoMode(winWidth, winHeight), "Sudoku", sf::Style::Close))
{
    m_Board = std::make_unique<Board>(9, sf::Vector2f(60.0f, 60.0f));
    SetBackground("res/images/background_gray.jpg");
}

Game::~Game()
{

}

void Game::Run()
{
    m_Window.setFramerateLimit(m_Fps);
    while (m_Window.isOpen())
    {
        // Event handling
        sf::Event event;
        while (m_Window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                m_Window.close();
                return;
            default:
                break;
            }
        }

        m_Window.clear(sf::Color(42, 42, 42, 1)); // dark grey

        // Draw calls
        m_Window.draw(m_Background);
        m_Board->Draw(m_Window);

        m_Window.display();
    }
}

void Game::SetBackground(const std::string& path)
{
    sf::Texture texture;
    if (texture.loadFromFile(path))
    {
        texture.loadFromFile(path);
        m_Background = sf::Sprite(texture);
        m_Background.setColor(sf::Color(170, 170, 170));
    }
    else
    {
        // Default gray color
        m_Background.setColor(sf::Color(170, 170, 170));
    }
}

bool Game::IsValidBoard()
{
	return false;
}
