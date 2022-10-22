#include "Board.h"

Board::Board(int n, const sf::Vector2f& cellSize)
	: m_Size(n)
{
	m_Font.loadFromFile("res/fonts/nanumgothic_regular.ttf");

	// Set size of 3 rectangles defining the grid (-1.0f so that right and bottom edge overlap with cell border)
	sf::Vector2f frameSize(n * cellSize.x, n * cellSize.y);
	sf::Vector2f horiSize((n / 3.0f) * cellSize.x - 1.0f, n * cellSize.y);
	sf::Vector2f vertSize(n * cellSize.x, (n / 3.0f) * cellSize.y - 1.0f);

	// Half sizes
	sf::Vector2f halfFrameSize(0.5f * frameSize);
	sf::Vector2f halfHoriSize(0.5f * horiSize);
	sf::Vector2f halfVertSize(0.5f * vertSize);
	sf::Vector2f halfCellSize(0.5f * cellSize);

	m_Frame = sf::RectangleShape(frameSize);
	m_HoriRect = sf::RectangleShape(horiSize);
	m_VertRect = sf::RectangleShape(vertSize);

	// Set origin, position, outline of rectangles
	m_Frame.setOrigin(halfFrameSize);
	m_HoriRect.setOrigin(halfHoriSize);
	m_VertRect.setOrigin(halfVertSize);

	m_Frame.setPosition(halfFrameSize.x + halfCellSize.x, halfFrameSize.y + halfCellSize.y);
	m_HoriRect.setPosition(halfHoriSize.x + 3.5f * cellSize.x, halfHoriSize.y + halfCellSize.y);
	m_VertRect.setPosition(halfVertSize.x + halfCellSize.x, halfVertSize.y + 3.5f * cellSize.y);

	m_Frame.setOutlineColor(sf::Color(42, 42, 42));
	m_HoriRect.setOutlineColor(sf::Color(70, 50, 50));
	m_VertRect.setOutlineColor(sf::Color(50, 70, 50));

	m_Frame.setOutlineThickness(3.3f);
	m_HoriRect.setOutlineThickness(2.0f);
	m_VertRect.setOutlineThickness(2.0f);

	m_Frame.setFillColor(sf::Color::Transparent);
	m_HoriRect.setFillColor(sf::Color::Transparent);
	m_VertRect.setFillColor(sf::Color::Transparent);

	// Fill grid with cells
	m_Grid = new Cell*[m_Size];
	for (int row = 1; row < m_Size + 1; row++)
	{
		m_Grid[row - 1] = new Cell[m_Size];
		for (int col = 1; col < m_Size + 1; col++)
			m_Grid[row - 1][col - 1] = Cell(sf::Text("9", m_Font, 0.6f * cellSize.x), false, row, col, cellSize);
	}
}

Board::~Board()
{
	for (int i = 0; i < m_Size; i++)
		delete[] m_Grid[i];

	delete[] m_Grid;
}

void Board::Draw(sf::RenderWindow& window)
{
	// Draw cells and their content
	for (int row = 0; row < m_Size; row++)
	{
		for (int col = 0; col < m_Size; col++)
		{
			Cell& cell = m_Grid[row][col];
			window.draw(cell.shape);
			window.draw(cell.num);
		}
	}

	// Draw rectangles
	window.draw(m_HoriRect);
	window.draw(m_VertRect);
	window.draw(m_Frame);
}

void Board::SetCell(int row, int col, sf::Text&& text)
{
	m_Grid[row][col].num = std::move(text);
}
