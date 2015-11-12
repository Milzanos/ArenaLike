#include "TileMap.h"



TileMap::TileMap(string fileName, Window* window)
{
	this->window = window;

	texture = new sf::Texture;
	texture->loadFromFile(fileName + ".png");
	textureBackground = new sf::Texture;
	textureBackground->loadFromFile(fileName + 'b' + ".png");

	offsetHider = new sf::RectangleShape;
	offsetHider->setFillColor(sf::Color(0, 0, 0, 255));

	sprite = new sf::RectangleShape(sf::Vector2f(tileSize.x, tileSize.y)),
	sprite->setTexture(texture);
	spriteBackground = new sf::RectangleShape(sf::Vector2f(tileSize.x, tileSize.y));
	spriteBackground->setTexture(textureBackground);

	setScreenSize();
}


TileMap::~TileMap()
{
	delete sprite;
	delete texture;
	delete textureBackground;
}

int TileMap::returnTile(int y, int x)
{
	return map[y][x];
}

int TileMap::returnColMap(int y, int x)
{
	return colMap[y][x];
}

void TileMap::changeTile(int y, int x, int type)
{
	map[y][x] = type;
}

void TileMap::changeColTile(int y, int x, int type)
{
	colMap[y][x] = type;
}

void TileMap::drawMap()
{
	int temp;

	for (int row = 0; row < 18; row++)
		for (int pos = 0; pos < 32; pos++)
		{
			spriteBackground->setPosition(tileSize.x * float(pos), tileSize.y * float(row) + offset);
			spriteBackground->setTextureRect(sf::IntRect(background[row][pos] * 16, 0, 16, 16));
			window->draw(*spriteBackground);

			temp = map[row][pos];
			if (temp != 0)
			{
				int y = 0;

				while (temp > this->spritesPerRow)
					temp -= this->spritesPerRow,
					y++;

				sprite->setPosition(tileSize.x * float(pos), tileSize.y * float(row) + offset),
				sprite->setTextureRect(sf::IntRect((temp - 1) * 16, y * 16, 16, 16));

				window->draw(*sprite);
			}
		}
}

void TileMap::loadMap(string fileName)
{
	char ch;
	int pos = 0;
	int row = 0;
	fstream fin(fileName, fstream::in);
	while (fin >> noskipws >> ch)
	{
		if (ch != '\n')
		{
			map[row][pos] = ch - 65;

			switch (map[row][pos])
			{
			case(0):
				colMap[row][pos] = NOTHING;
				break;
			case(21) :
				colMap[row][pos] = JUMP;
				break;
			case(22) :
				colMap[row][pos] = SLOW;
				break;
			default:
				colMap[row][pos] = WALL;
				break;
			}

			if (map[row][pos] > 30)
				colMap[row][pos] = NOTHING;

			background[row][pos] = rand() % 4;

			pos++;
		}
		else
		{
			row++;
			pos = 0;
		}
	}
}

void TileMap::setScreenSize()
{
	tileSize = { window->screenSize.x / 32.f, 0.f};
	
	if (tileSize.x != int(tileSize.x))
		tileSize.x = float(int(tileSize.x));
	
	tileSize.y = tileSize.x;

	offset = window->screenSize.y - tileSize.y * 18.f;
	offset /= 2.f;

	if(sprite != NULL)
		sprite->setSize(sf::Vector2f(tileSize.x, tileSize.y));

	if(spriteBackground != NULL)
		spriteBackground->setSize(sf::Vector2f(tileSize.x, tileSize.y));

	if (offsetHider != NULL)
		offsetHider->setSize(sf::Vector2f(window->screenSize.x, offset));
}

void TileMap::hideOffset()
{
	offsetHider->setPosition(0, 0);
	window->draw(*offsetHider);
	offsetHider->setPosition(0, window->screenSize.y - offset);
	window->draw(*offsetHider);
}