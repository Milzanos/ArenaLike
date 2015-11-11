#include "Bullet.h"



Bullet::Bullet(Window* window, TileMap* map, hv::position position, hv::position speed, float damage, float timeLeft)
{
	this->window = window;
	this->map = map;
	this->position = position;
	this->speed = speed;
	this->damage = damage;
	this->timeLeft = timeLeft;

	texture = new sf::Texture;
	texture->loadFromFile("../resources/images/bullet.png");

	body = new sf::RectangleShape();
	setScreenSize();
	body->setTexture(texture);
}

Bullet::~Bullet()
{
	delete texture;
	delete body;
}

void Bullet::update()
{
	timeLeft -= 1.f / 60.f;

	if (timeLeft <= 0)
		dead = true;

	position.add(speed);
	body->setPosition(position.x, position.y);
}

void Bullet::collision()
{
	hv::position temp(float(int(position.x / map->tileSize.x)), float(int(position.y / map->tileSize.y)));

	for (int row = -1; row < 3; row++)
		for (int pos = -1; pos < 3; pos++)
		{
			if (map->returnColMap(row + temp.y , pos + temp.x) != map->NOTHING)
			{
				if (collisionCheck({ (pos + temp.x) * map->tileSize.x, (row + temp.y) * map->tileSize.y + map->offset}))
				{
					dead = true;
				}
			}
		}
}

void Bullet::draw()
{
	window->draw(*body);
}

bool Bullet::collisionCheck(hv::position other)
{
	hv::size size = { map->tileSize.x, map->tileSize.y };

	//XCHECK
	if (position.x - size.x < other.x &&
		position.x + body->getSize().x > other.x &&
		position.y - size.y < other.y &&
		position.y + body->getSize().y > other.y)
		return true;

	return false;
}

void Bullet::setScreenSize()
{
	size = { map->tileSize.x / timesSmaller, map->tileSize.y / timesSmaller };
	body->setSize(sf::Vector2f(size.x, size.y));
}