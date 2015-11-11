#include "Base.h"



Base::Base(Window* window, TileMap* map, hv::position position, string fileName)
{
	this->window = window;
	this->map = map;

	texture = new sf::Texture;
	texture->loadFromFile(fileName);

	body = new sf::RectangleShape();
	body->setTexture(texture);
	body->setPosition(position.x, position.y);
	setScreenSize();
}


Base::~Base()
{
	delete body;
	delete texture;
}

void Base::setPosition(float x, float y)
{
	body->setPosition(x, y - map->offset);
}

void Base::draw()
{
	window->draw(*body);
}

void Base::setScreenSize(float specialSize)
{
	hv::size temp = { map->tileSize.x * specialSize, 0.f };

	if (temp.x != int(temp.x))
		temp.x = float(int(temp.x));
	temp.y = temp.x;

	body->setSize({ temp.x, temp.y });
	size = { body->getSize().x, body->getSize().x };
}

void Base::setUVRect(int x, int y)
{
	int tempX = x, tempY = 0;

	if (y == 0)
	{
		while (tempX >= 4)
			tempY++,
			tempX -= 4;
	}
	if (y == 1)
	{
		while (tempX >= 7)
			tempY++,
			tempX -= 7;
	}

	body->setTextureRect(sf::IntRect(10 * tempX, 16 * tempY, 10, 16));
}

void Base::changeAlpha(int a)
{
	body->setFillColor(sf::Color(255, 255, 255, a));
}

void Base::setColor(int r, int g, int b, int a)
{
	body->setFillColor(sf::Color(r, g, b, a));
}