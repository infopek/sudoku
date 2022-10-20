#include "Board.h"

Board::Board(int n, const sf::Vector2f& cellSize)
	: m_Size(n)
{
	sf::Vector2f frameSize(n * cellSize.x, n * cellSize.y);
	sf::Vector2f horiSize((n / 3) * cellSize.x - m_Padding, n * cellSize.y);
	sf::Vector2f vertSize(n * cellSize.x, (n / 3) * cellSize.y - m_Padding);

	sf::Vector2f halfFrameSize(frameSize / 2.0f);
	sf::Vector2f halfHoriSize(horiSize / 2.0f);
	sf::Vector2f halfVertSize(vertSize / 2.0f);

	m_Frame = sf::RectangleShape(frameSize);
	m_HoriRect = sf::RectangleShape(horiSize);
	m_VertRect = sf::RectangleShape(vertSize);

	m_Frame.setOrigin(halfFrameSize);
	m_HoriRect.setOrigin(halfHoriSize);
	m_VertRect.setOrigin(halfVertSize);

	m_Frame.setPosition(halfFrameSize.x + m_Padding * cellSize.x, halfFrameSize.y + m_Padding * cellSize.y);
	m_HoriRect.setPosition(halfHoriSize.x + (3 + m_Padding) * cellSize.x, halfHoriSize.y + m_Padding * cellSize.y);
	m_VertRect.setPosition(halfVertSize.x + m_Padding * cellSize.x, halfVertSize.y + (3 + m_Padding) * cellSize.y);

	m_Frame.setOutlineColor(sf::Color(42, 42, 42));
	m_Frame.setOutlineThickness(3.3f);
	m_HoriRect.setOutlineColor(sf::Color(70, 50, 50));
	m_HoriRect.setOutlineThickness(2.0f);
	m_VertRect.setOutlineColor(sf::Color(50, 70, 50));
	m_VertRect.setOutlineThickness(2.0f);

	m_Frame.setFillColor(sf::Color::Transparent);
	m_HoriRect.setFillColor(sf::Color::Transparent);
	m_VertRect.setFillColor(sf::Color::Transparent);

	m_Board = new Cell*[m_Size];
	for (int row = 0; row < m_Size; row++)
	{
		m_Board[row] = new Cell[m_Size];
		for (int col = 0; col < m_Size; col++)
		{
			m_Board[row][col] = Cell(-1, false, row, col, cellSize);
		}
	}
}

Board::~Board()
{
	delete[] m_Board;
}

void Board::Draw(sf::RenderWindow& window)
{
	for (int row = 0; row < m_Size; row++)
	{
		for (int col = 0; col < m_Size; col++)
		{
			window.draw(m_Board[row][col].shape);
		}
	}

	window.draw(m_HoriRect);
	window.draw(m_VertRect);
	window.draw(m_Frame);
}

bool Board::IsValid()
{
	return true;
}