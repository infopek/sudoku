#include <iostream>
#include <cstring>
#include <numeric>

#include <vector>
#include <unordered_map>

#include <SFML/Graphics.hpp>

namespace std 
{
	template<typename X, typename Y>
	struct hash<std::pair<X, Y>>
	{
		std::size_t operator()(const std::pair<X, Y>& pair) const 
		{
			return std::hash<X>()(pair.first) ^ std::hash<Y>()(pair.second);
		}
	};
}


struct Cell
{
	int row;
	int col;

	sf::RectangleShape shape;
	sf::Text text;

	Cell()
		: row(-1), col(-1) {}
	Cell(const sf::RectangleShape& shape, const sf::Text& text, int row, int col)
		: shape(shape), text(text), row(row), col(col) {}
};

// Window
constexpr unsigned int WIN_WIDTH = 1280;
constexpr unsigned int WIN_HEIGHT = 720;
constexpr int FPS = 60;

// Board
constexpr int N = 9;
Cell board[N][N];
Cell* brdSelected = nullptr;
std::vector<std::unordered_map<std::pair<float, float>, Cell*>> sameNumbers(N + 1);

sf::RectangleShape boardFrame;
constexpr float CELLSIZE = 55.0f;
constexpr float HALF_CELLSIZE = 0.5f * CELLSIZE;

constexpr unsigned int FONTSIZE = 36;

// Numpad
constexpr int M = 3;
Cell numpad[M][M];
Cell* npSelected = nullptr;

sf::RectangleShape npFrame;
constexpr float NP_CELLSIZE = 110.0f;
constexpr float NP_HALF_CELLSIZE = 0.5f * NP_CELLSIZE;
constexpr float PADDING = 10.0f;

constexpr unsigned int NP_FONTSIZE = 42;

// Initialize board and numpad
void FillBoardRandom(const sf::Font& font)
{
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
		{
			sf::RectangleShape square(sf::Vector2f(CELLSIZE, CELLSIZE));
			square.setOrigin(0.5f * square.getSize());
			square.setPosition(sf::Vector2f(CELLSIZE * j, CELLSIZE * i));

			square.setFillColor(sf::Color(240, 240, 240));
			square.setOutlineThickness(1.0f);
			square.setOutlineColor(sf::Color(192, 192, 192));

			// Sample box so that every number is in the same position relative to cell
			sf::FloatRect numBounds = sf::Text("0", font, FONTSIZE).getLocalBounds();

			int randomNum;
			if (rand() % 5 == 0)
				randomNum = rand() % N + 1;
			else
				randomNum = 0;

			// Number inside the cell
			sf::Text num((char)(randomNum + '0'), font, FONTSIZE);
			num.setOrigin(0.5f * numBounds.width, 0.5f * numBounds.height);
			num.setPosition(square.getPosition().x - 0.05f * CELLSIZE, square.getPosition().y - 0.17f * CELLSIZE);

			num.setFillColor(sf::Color(42, 42, 42));
			board[i - 1][j - 1] = Cell(square, num, i - 1, j - 1);

			// Store position of cell
			sf::Vector2f pos = board[i - 1][j - 1].shape.getPosition();
			std::pair<float, float> coords = { pos.x, pos.y };
			sameNumbers[randomNum][coords] = &board[i - 1][j - 1];
		}
	}
}
void FillNumpad(const sf::Font& font)
{
	int curr = 1;
	for (int i = 1; i <= M; i++)
	{
		for (int j = 1; j <= M; j++)
		{
			sf::RectangleShape square(sf::Vector2f(NP_CELLSIZE - PADDING, NP_CELLSIZE - PADDING));
			square.setOrigin(0.5f * square.getSize());
			square.setPosition(sf::Vector2f((N * CELLSIZE) + NP_CELLSIZE * j, NP_CELLSIZE * i + 0.5f * M * NP_CELLSIZE - HALF_CELLSIZE));

			square.setFillColor(sf::Color(255, 229, 204));

			// Sample box so that every number is in the same position relative to cell
			sf::FloatRect numBounds = sf::Text("0", font, NP_FONTSIZE).getLocalBounds();

			sf::Text num(std::to_string(curr++), font, NP_FONTSIZE);
			num.setOrigin(0.5f * numBounds.width, 0.5f * numBounds.height);
			num.setPosition(square.getPosition().x - 0.07f * NP_HALF_CELLSIZE, square.getPosition().y - 0.17f * NP_HALF_CELLSIZE);

			num.setFillColor(sf::Color(255, 128, 0));

			numpad[i - 1][j - 1] = Cell(square, num, i - 1, j - 1);
		}
	}
}

// Draw routines
void DrawBoard(sf::RenderWindow& window, const sf::Font& font)
{
	// Set size of 3 rectangles defining the grid (-1.0f so that right and bottom edge overlap with cell border)
	sf::Vector2f frameSize(N * CELLSIZE - 1.0f, N * CELLSIZE - 1.0f);
	sf::Vector2f horiSize((N / 3.0f) * CELLSIZE - 1.0f, N * CELLSIZE);
	sf::Vector2f vertSize(N * CELLSIZE, (N / 3.0f) * CELLSIZE - 1.0f);

	// Half sizes
	sf::Vector2f halfFrameSize(0.5f * frameSize);
	sf::Vector2f halfHoriSize(0.5f * horiSize);
	sf::Vector2f halfVertSize(0.5f * vertSize);

	// Rectangles
	boardFrame = sf::RectangleShape(frameSize);
	sf::RectangleShape horiRect(horiSize);
	sf::RectangleShape vertRect(vertSize);

	// Set origin, position, outline of rectangles
	boardFrame.setOrigin(halfFrameSize);
	horiRect.setOrigin(halfHoriSize);
	vertRect.setOrigin(halfVertSize);

	boardFrame.setPosition(halfFrameSize.x + HALF_CELLSIZE, halfFrameSize.y + HALF_CELLSIZE);
	horiRect.setPosition(halfHoriSize.x + 3.5f * CELLSIZE, halfHoriSize.y + HALF_CELLSIZE);
	vertRect.setPosition(halfVertSize.x + HALF_CELLSIZE, halfVertSize.y + 3.5f * CELLSIZE);

	boardFrame.setOutlineColor(sf::Color(30, 30, 30));
	horiRect.setOutlineColor(sf::Color(50, 50, 50));
	vertRect.setOutlineColor(sf::Color(50, 50, 50));

	boardFrame.setOutlineThickness(3.0f);
	horiRect.setOutlineThickness(2.0f);
	vertRect.setOutlineThickness(2.0f);

	boardFrame.setFillColor(sf::Color::Transparent);
	horiRect.setFillColor(sf::Color::Transparent);
	vertRect.setFillColor(sf::Color::Transparent);

	// Draw cells
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
		{
			window.draw(board[i - 1][j - 1].shape);
			if (board[i - 1][j - 1].text.getString()[0] > '0')
			window.draw(board[i - 1][j - 1].text);
		}
	}

	window.draw(vertRect);
	window.draw(horiRect);
	window.draw(boardFrame);
}
void DrawNumpad(sf::RenderWindow& window, const sf::Font& font)
{
	const unsigned int padding = 10.0f;
	sf::Vector2f frameSize(M * NP_CELLSIZE, M * NP_CELLSIZE);
	sf::Vector2f halfFrameSize(0.5f * frameSize);

	npFrame = sf::RectangleShape(frameSize);

	npFrame.setOrigin(halfFrameSize);
	npFrame.setPosition((N * CELLSIZE) + halfFrameSize.x + NP_HALF_CELLSIZE, 3.5f * CELLSIZE + halfFrameSize.y);

	npFrame.setFillColor(sf::Color::Transparent);

	// Draw cells
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < M; j++)
		{
			window.draw(numpad[i][j].shape);
			window.draw(numpad[i][j].text);
		}
	}

	window.draw(npFrame);
}

// Select and deselect a cell, its row, col, box and all the numbers that are equal to its content on the board
void SelectCells(Cell* cell)
{
	int row = cell->row;
	int col = cell->col;

	// Highlight clicked cell
	cell->shape.setFillColor(sf::Color(240, 200, 100));

	// Highlight row and col, slightly lighter color
	for (int n = 0; n < N; n++)
	{
		if (n != col)
			board[row][n].shape.setFillColor(sf::Color(250, 240, 190));
		if (n != row)
			board[n][col].shape.setFillColor(sf::Color(250, 240, 190));
		}

	int boxRow = row / (N / 3) * 3;
	int boxCol = col / (N / 3) * 3;

	// Highlight sub box
	for (int i = boxRow; i < boxRow + N / 3; i++)
		for (int j = boxCol; j < boxCol + N / 3; j++)
			if (!(i == row && j == col))
				board[i][j].shape.setFillColor(sf::Color(250, 240, 190));

	// Highlight numbers that are equal to clicked cell
	std::unordered_map<std::pair<float, float>, Cell*>& cells = sameNumbers[cell->text.getString()[0] - '0'];
	if (cell->text.getString()[0] > '0')
	{
		for (auto it = cells.begin(); it != cells.end(); it++)
		{
			Cell* c = it->second;
			if (c && !(c->row == row && c->col == col))
				c->shape.setFillColor(sf::Color(240, 180, 30));
	}
	}
}
void DeselectCells(Cell* cell)
{
	int row = cell->row;
	int col = cell->col;

	// Deselect clicked cell
	cell->shape.setFillColor(sf::Color(240, 240, 240));

	// Deselect row, col
	for (int z = 0; z < N; z++)
	{
		if (z != col)
			board[row][z].shape.setFillColor(sf::Color(240, 240, 240));
		if (z != row)
			board[z][col].shape.setFillColor(sf::Color(240, 240, 240));
	}

	int boxRow = row / (N / 3) * 3;
	int boxCol = col / (N / 3) * 3;

	// Deselect sub box
	for (int i = boxRow; i < boxRow + N / 3; i++)
		for (int j = boxCol; j < boxCol + N / 3; j++)
			if (!(i == row && j == col))
				board[i][j].shape.setFillColor(sf::Color(240, 240, 240));

	// Deselect numbers that are equal to clicked cell
	if (cell->text.getString()[0] > '0')
		for (Cell* c : sameNumbers[cell->text.getString()[0] - '0'])
			if (c && !(c->row == row && c->col == col))
				c->shape.setFillColor(sf::Color(240, 240, 240));
}

int main()
{
	// Random
	srand(time(0));

	// Window
	sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Sudoku", sf::Style::Close);
	window.setFramerateLimit(FPS);

	// Background
	sf::Texture texture;
	texture.loadFromFile("res/images/background_orange.jpg");
	sf::Sprite background(texture);

	// Font
	sf::Font regFont;
	sf::Font boldFont;
	regFont.loadFromFile("res/fonts/nanumgothic_regular.ttf");
	boldFont.loadFromFile("res/fonts/nanumgothic_bold.ttf");

	FillBoardRandom(regFont);
	FillNumpad(boldFont);

	while (window.isOpen())
	{
		sf::Vector2f mousePos(sf::Mouse::getPosition(window));
		sf::Event event;

		// Event handling
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
			{
				// Clicked inside the board
				if (boardFrame.getGlobalBounds().contains(mousePos))
				{
					if (brdSelected)
						DeselectCells(brdSelected);

					bool found = false;
					// Improve: speed (currently O(n^2))
					for (int i = 0; i < N; i++)
					{
						for (int j = 0; j < N; j++)
						{
							if (board[i][j].shape.getGlobalBounds().contains(mousePos) && !found)
							{
								brdSelected = &board[i][j];
								SelectCells(brdSelected);
								found = true;
							}
						}
					}
				}
			}
		}

		window.clear();

		// Draw calls
		window.draw(background);
		DrawBoard(window, regFont);
		DrawNumpad(window, boldFont);

		window.display();
	}
}