#pragma once

#include <cstring>

#include <SFML/Graphics.hpp>

class Board
{
public:
	Board(int n, const sf::Vector2f& cellSize);
	~Board();

	void Draw(sf::RenderWindow& window);
	bool IsValid();

public:
	struct Cell
	{
		int val;
		bool selected;

		sf::RectangleShape shape;

		Cell() {}
		Cell(int val, bool selected, int row, int col, const sf::Vector2f& cellSize)
			: val(val), selected(selected)
		{
			shape = sf::RectangleShape(cellSize);
			//shape.setOrigin(cellSize / 2.0f);
			shape.setPosition(sf::Vector2f(cellSize.x * (row + m_Padding), cellSize.y * (col + m_Padding)));
			shape.setFillColor(sf::Color(240, 240, 240));

			shape.setOutlineThickness(1.0f);
			shape.setOutlineColor(sf::Color(192, 192, 192));
		}
	};

private:
	static constexpr float m_Padding = 1.0f;

	int m_Size;
	Cell** m_Board;

	sf::Vector2f m_CellSize;

	sf::RectangleShape m_Frame;
	sf::RectangleShape m_HoriRect;
	sf::RectangleShape m_VertRect;
};

