#pragma once

#include <SFML/Graphics.hpp>

#include "Board.h"

class Game
{
public:
	Game(unsigned int winWidth, unsigned int winHeight);
	~Game();

	void Run();

	void SetBackground(const std::string& path);
private:

	bool IsValidBoard();
private:
	sf::RenderWindow m_Window;
	sf::Sprite m_Background;

	unsigned int m_Fps;

	std::unique_ptr<Board> m_Board;
};

