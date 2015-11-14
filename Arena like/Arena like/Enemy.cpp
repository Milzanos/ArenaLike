#include "Enemy.h"



Enemy::Enemy(Window* window, TileMap* map, hv::position position, int type)
{
	this->window = window;
	this->map = map;
	this->position = position;

	left = rand() % 2;

	body = new sf::RectangleShape;
	texture = new sf::Texture;
	texture->loadFromFile("../resources/images/enemy" + to_string(type + 1) + ".png");
	body->setTexture(texture);

	setScreenSize();

	if (type == 0)
		health = 1.f,
		body->setSize(sf::Vector2f(map->tileSize.x, map->tileSize.y)),
		maxSpeed *= 1.5f,
		colChecks = 3,
		enemyUVSize = 16;
	if (type == 1)
		health = 5.f,
		body->setSize(sf::Vector2f(map->tileSize.x * 2, map->tileSize.y * 2)),
		maxSpeed *= 1.0f,
		colChecks = 4,
		enemyUVSize = 32;
	if (type == 2)
		health = 10.f,
		body->setSize(sf::Vector2f(map->tileSize.x * 3, map->tileSize.y * 3)),
		maxSpeed *= 0.5f,
		colChecks = 5,
		enemyUVSize = 48;

	position.x -= body->getSize().x / 2.f;
}


Enemy::~Enemy()
{
	delete texture;
	delete body;
}

void Enemy::update()
{
	checkMovement();
	collision();
	doMovement();
	updateTexture();

	if (dying)
	{
		dyingTimer -= 1.f / 60.f;

		if (dyingTimer <= 0.f)
			dyingTimer = 0.f,
			dead = true;

		body->setFillColor(sf::Color(255, 255, 255, 255 * (dyingTimer * 5.f)));
	}
	else
	{
		if (damageTimer < damageSwitch)
			damageTimer += 1.f / 60.f,
			body->setFillColor(sf::Color(255, 0, 0, 255));
		else
			body->setFillColor(sf::Color(255, 255, 255, 255));
	}
}

void Enemy::checkMovement()
{
	if (speed.y < gravity)
		speed.y += gravityAcc;

	if (left && speed.x > -maxSpeed)
		speed.x -= speedAcc;
	else if (speed.x < maxSpeed)
		speed.x += speedAcc;

	if (speed.x > 0.1f)
		speed.x -= speedAcc / 2.f;
	else if (speed.x < -0.1f)
		speed.x += speedAcc / 2.f;
	else
		speed.x = 0.f;
}

void Enemy::collision()
{
	hv::position enemyPos(float(int(position.x / map->tileSize.x)), float(int(position.y / map->tileSize.y)));

	bool resetY = false;
	bool megaJump = false;

	for (int row = -2; row < colChecks; row++)
		for (int pos = -2; pos < colChecks; pos++)
			if (row + enemyPos.y >= 0 && pos + enemyPos.x >= 0 && row + enemyPos.y < 18 && pos + enemyPos.x < 32)
			{
				if (map->returnColMap(row + enemyPos.y, pos + enemyPos.x) != map->NOTHING)
				{
					hv::bVec3 temp = collisionCheck({ (pos + enemyPos.x) * map->tileSize.x, (row + enemyPos.y) * map->tileSize.y + map->offset });

					if (temp.x)
						speed.x = 0,
						left = !left;

					if (temp.y)
						resetY = true;

					if (temp.z)
					{
						if (map->returnColMap(enemyPos.y + row, enemyPos.x + pos) == map->JUMP)
							speed.y = -jumpHeight,
							megaJump = true;

						if (map->returnColMap(enemyPos.y + row, enemyPos.x + pos) == map->SLOW)
							speed.x /= 2;
					}

					if (temp.x && temp.y)
					{
						hv::position tPosP = position;
						tPosP.add(speed);
						hv::position tPosO = { (pos + enemyPos.x) * map->tileSize.x, (row + enemyPos.y) * map->tileSize.y + map->offset };

						if (tPosP.x > tPosO.x)
							position.x++;
						else if (tPosP.x < tPosO.x)
							position.x--;
						if (tPosP.y > tPosO.y)
							position.y++;
						else if (tPosP.y < tPosO.y)
							position.y--;
					}
				}
			}

	if(resetY && !megaJump)
		speed.y = 0;
}

hv::bVec3 Enemy::collisionCheck(hv::position other)
{
	hv::bVec3 bVec = { false, false, false };
	hv::position temp = position;
	temp.add(speed);

	hv::position cX = { temp.x, position.y };
	hv::position cY = { position.x, temp.y };

	if (cX.x - map->tileSize.x < other.x &&
		cX.x + body->getSize().x > other.x &&
		cX.y - map->tileSize.y < other.y &&
		cX.y + body->getSize().y > other.y)
		bVec.x = true;

	if (cY.x - map->tileSize.x < other.x &&
		cY.x + body->getSize().x > other.x &&
		cY.y - map->tileSize.y < other.y &&
		cY.y + body->getSize().y > other.y)
		bVec.y = true;

	if (cY.x - map->tileSize.x < other.x &&
		cY.x + body->getSize().x > other.x &&
		cY.y < other.y &&
		cY.y + body->getSize().y > other.y)
		bVec.z = true;

	return bVec;
}

void Enemy::doMovement()
{
	position.add(speed);

	if (position.y > window->screenSize.y - map->offset)
	{
		if (normal)
			normal = false,
			maxSpeed *= 1.5,
			aniTimer /= 2;

		position.x = window->screenSize.x / 2 - body->getSize().x / 2;
		position.y = -body->getSize().y;
		speed.y = gravity;
	}

	body->setPosition(position.x, position.y);
}

void Enemy::updateTexture()
{
	aniTimer += 1.f / 60.f;
	
	if (aniTimer >= aniSwitch)
	{
		aniTimer = 0.f;
		ani++;

		if (ani > 1)
			ani = 0;
	}

	if(speed.y == 0)
		if(normal)
			body->setTextureRect(sf::IntRect(ani * enemyUVSize, int(left) * enemyUVSize, enemyUVSize, enemyUVSize));
		else
			body->setTextureRect(sf::IntRect((ani + 2) * enemyUVSize, int(left) * enemyUVSize, enemyUVSize, enemyUVSize));
	else
		if (normal)
			body->setTextureRect(sf::IntRect(int(left) * enemyUVSize, enemyUVSize * 2, enemyUVSize, enemyUVSize));
		else
			body->setTextureRect(sf::IntRect(int(left + 2) * enemyUVSize, enemyUVSize * 2, enemyUVSize, enemyUVSize));
}

void Enemy::draw()
{
	window->draw(*body);
}

void Enemy::damage(float dmg)
{
	health -= dmg;

	if (health <= 0)
		dying = true;

	damageTimer = 0.f;
}

void Enemy::setScreenSize()
{
	speedAcc = map->tileSize.x / baseSpeedAcc;
	maxSpeed = map->tileSize.x / baseMaxSpeed;
	gravity = map->tileSize.y / baseGravity;
	gravityAcc = map->tileSize.y / baseGravityAcc;
	jumpHeight = map->tileSize.y / baseJumpHeight;

	body->setSize({ map->tileSize.x, map->tileSize.y });
}

hv::size Enemy::getSize()
{
	hv::size temp = { body->getSize().x, body->getSize().y };
	return temp;
}