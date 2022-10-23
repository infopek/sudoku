#include <iostream>
#include <cstring>
#include <numeric>

#include <SFML/Graphics.hpp>

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

sf::RectangleShape boardFrame;
constexpr float CELLSIZE = 55.0f;
constexpr float HALF_CELLSIZE = 0.5f * CELLSIZE;

constexpr unsigned int FONTSIZE = 36;

// Numpad
constexpr int M = 3;
int numpad[M][M] =
{
	{ 1, 2, 3 },
	{ 4, 5, 6 },
	{ 7, 8, 9 }
};

sf::RectangleShape npFrame;
constexpr float NP_CELLSIZE = 110.0f;
constexpr float NP_HALF_CELLSIZE = 0.5f * NP_CELLSIZE;

constexpr unsigned int NP_FONTSIZE = 42;

void FillBoardRandom(const sf::Font& font)
{
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
		{
			sf::RectangleShape square(sf::Vector2f(CELLSIZE, CELLSIZE));
			square.setOrigin(0.5f * square.getSize());
			square.setPosition(sf::Vector2f(CELLSIZE * i, CELLSIZE * j));

			square.setFillColor(sf::Color(240, 240, 240));
			square.setOutlineThickness(1.0f);
			square.setOutlineColor(sf::Color(192, 192, 192));

			// Sample box so that every number is in the same position relative to cell
			sf::FloatRect box = sf::Text("0", font, FONTSIZE).getLocalBounds();

			sf::Text num((char)(rand() % N + 1 + '0'), font, FONTSIZE);
			num.setOrigin(0.5f * box.width, 0.5f * box.height);
			num.setPosition(square.getPosition().x - 0.05f * CELLSIZE, square.getPosition().y - 0.17f * CELLSIZE);

			num.setFillColor(sf::Color(42, 42, 42));

			board[i - 1][j - 1] = Cell(square, num, i - 1, j - 1);
		}
	}
}

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
	for (int i = 1; i <= M; i++)
	{
		for (int j = 1; j <= M; j++)
		{
			sf::RectangleShape cell(sf::Vector2f(NP_CELLSIZE - padding, NP_CELLSIZE - padding));
			cell.setOrigin(0.5f * cell.getSize());
			cell.setPosition(sf::Vector2f((N * CELLSIZE) + NP_CELLSIZE * i, NP_CELLSIZE * j + halfFrameSize.y - HALF_CELLSIZE));

			cell.setFillColor(sf::Color(255, 229, 204));

			// Sample box so that every number is in the same position relative to cell
			sf::FloatRect box = sf::Text("0", font, NP_FONTSIZE).getLocalBounds();

			sf::Text num(std::to_string(numpad[j - 1][i - 1]), font, NP_FONTSIZE);
			num.setOrigin(0.5f * box.width, 0.5f * box.height);
			num.setPosition(cell.getPosition().x - 0.07f * NP_HALF_CELLSIZE, cell.getPosition().y - 0.17f * NP_HALF_CELLSIZE);

			num.setFillColor(sf::Color(255, 128, 0));

			window.draw(cell);
			window.draw(num);
		}
	}

	window.draw(frame);
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
	regFont.loadFromFile("res/fonts/nanumgothic_regular.ttf");

	sf::Font boldFont;
	boldFont.loadFromFile("res/fonts/nanumgothic_bold.ttf");

	FillBoardRandom(regFont);

	while (window.isOpen())
	{
		// Event handling
		sf::Vector2f mousePos(sf::Mouse::getPosition(window));
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
				;
		}

		window.clear();

		// Draw calls
		window.draw(background);
		DrawBoard(window, regFont);
		DrawNumpad(window, boldFont);

		window.display();
	}
}