#pragma once

#include <cstring>

#include <SFML/Graphics.hpp>

class Board
{
public:
	Board() {}
	Board(int n, const sf::Vector2f& cellSize);
	~Board();

	void Draw(sf::RenderWindow& window);
	void SetCell(int row, int col, sf::Text&& text);

private:
	struct Cell
	{
		sf::Text num;
		bool selected;

		int row;
		int col;

		sf::RectangleShape shape;

		Cell() {}
		Cell(sf::Text&& text, bool selected, int row, int col, const sf::Vector2f& cellSize)
			: num(std::move(text)), selected(selected), row(row), col(col)
		{
			shape = sf::RectangleShape(cellSize);
			shape.setOrigin(0.5f * cellSize);
			shape.setPosition(sf::Vector2f(cellSize.x * row, cellSize.y * col));

			shape.setFillColor(sf::Color(240, 240, 240));
			shape.setOutlineThickness(1.0f);
			shape.setOutlineColor(sf::Color(192, 192, 192));

			num.setOrigin(0.5f * num.getLocalBounds().width, 0.5f * num.getLocalBounds().height);
			num.setPosition(shape.getPosition().x - 0.05f * cellSize.x, shape.getPosition().y - 0.17f * cellSize.y);

			num.setFillColor(sf::Color(42, 42, 42));
		}
	};

private:
	int m_Size;
	Cell** m_Grid;

	sf::Font m_Font;

	sf::RectangleShape m_Frame;
	sf::RectangleShape m_HoriRect;
	sf::RectangleShape m_VertRect;
};

