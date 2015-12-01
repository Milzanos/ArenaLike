#include "LevelEditor.h"

LevelEditor::LevelEditor(string fileName, Window* window)
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

LevelEditor::~LevelEditor()
{
	delete sprite;
	delete texture;
	delete textureBackground;
}

void LevelEditor::update()
{
	hv::position mousePos = window->getMousePosition();

	mousePos.x = float(int(mousePos.x / tileSize.x));
	mousePos.y = float(int(mousePos.y / tileSize.x));

	if (!blockSelection(mousePos))
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (map[int(mousePos.y)][int(mousePos.x)] != selectedBlock)
			{
				map[int(mousePos.y)][int(mousePos.x)] = selectedBlock;
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			if (map[int(mousePos.y)][int(mousePos.x)] != 0)
			{
				map[int(mousePos.y)][int(mousePos.x)] = 0;
			}
		}
	}
}

void LevelEditor::drawMap()
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
	sf::RectangleShape tempSprite(*sprite);

	for (int i = 0; i < 18; i++)
		for (int j = 0; j < 2; j++)
		{
			tempSprite.setPosition((offsetSelection.x + i) * tileSize.x, (offsetSelection.y + j) * tileSize.x + offset);
			tempSprite.setTextureRect(sf::IntRect(i * 16, j * 16, 16, 16));
			window->draw(tempSprite);
		}
}

void LevelEditor::loadMap(string fileName)
{
	char ch;
	int pos = 0;
	int row = 0;
	fstream fin(fileName, fstream::in);
	this->fileName = fileName;
	while (fin >> noskipws >> ch)
	{
		if (ch != '\n')
		{
			map[row][pos] = ch - 65;

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

void LevelEditor::saveMap()
{
	string tempString;
	fstream fin(fileName, fstream::out);
	if (!fin.is_open())
		cout << "FAILED: " << fileName << endl;

	fin.clear();
	for (int i = 0; i < 18; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			int temp = returnTile(i, j) + 'A';
			tempString += temp;
		}

		if (i != 17)
			fin << tempString + "\n";
		else
			fin << tempString;
		tempString.clear();
	}

}

void LevelEditor::setScreenSize()
{
	tileSize = { window->screenSize.x / 32.f, 0.f };

	if (tileSize.x != int(tileSize.x))
		tileSize.x = float(int(tileSize.x));

	tileSize.y = tileSize.x;

	offset = window->screenSize.y - tileSize.y * 18.f;
	offset /= 2.f;

	if (sprite != NULL)
		sprite->setSize(sf::Vector2f(tileSize.x, tileSize.y));

	if (spriteBackground != NULL)
		spriteBackground->setSize(sf::Vector2f(tileSize.x, tileSize.y));

	if (offsetHider != NULL)
		offsetHider->setSize(sf::Vector2f(window->screenSize.x, offset));

	offsetSelection.x = 7.f;
	offsetSelection.y = 1.f;
}

void LevelEditor::hideOffset()
{
	offsetHider->setPosition(0, 0);
	window->draw(*offsetHider);
	offsetHider->setPosition(0, window->screenSize.y - offset);
	window->draw(*offsetHider);
}

bool LevelEditor::blockSelection(hv::position mousePos)
{

	for (int i = 0; i < 18; i++)
		for (int j = 0; j < 2; j++)
		{
			if (mousePos.x >= offsetSelection.x + i - 1 && mousePos.x <= offsetSelection.x + i &&
				mousePos.y >= offsetSelection.y + j - 1 + offset && mousePos.y <= offsetSelection.y + j + offset)
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					selectedBlock = i + j * 18 + 1;
					return true;
				}
			}
		}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		offsetSelection.x = 7.f;
		offsetSelection.y = 1.f;
		return true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		offsetSelection.x = 7.f;
		offsetSelection.y = 15.f;
		return true;
	}

	return false;
}

int LevelEditor::returnTile(int y, int x)
{
	return map[y][x];
}