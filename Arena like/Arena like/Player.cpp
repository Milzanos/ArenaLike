#include "Player.h"



Player::Player(int lvl, Window* window, hv::position position)
{
	map = new TileMap("../resources/images/sslvl" + to_string(lvl), window);
	map->loadMap("../resources/maps/map" + to_string(lvl) + ".MAP");
	this->window = window;
	this->position = { window->screenSize.x * position.x, window->screenSize.y * position.y + map->offset};
	this->startPosition = this->position;
	speedAcc = map->tileSize.x / speedAcc;
	maxSpeed = map->tileSize.x / maxSpeed;
	gravity = map->tileSize.y / gravity;
	gravityAcc = map->tileSize.y / gravityAcc;
	jumpHeight = map->tileSize.y / jumpHeight;

	knockbackDevider /= map->tileSize.x;

	maxSpread = map->tileSize.y / maxSpread;

	letter = new Base(window, map, { window->screenSize.x / 2 - map->tileSize.x / 2, map->tileSize.y * 4 + map->offset * 2 }, "../resources/images/letters.png");
	number = new Base(window, map, { window->screenSize.x / 2 - map->tileSize.x / 2, map->tileSize.y * 4 + map->offset * 2 }, "../resources/images/numbers.png");
	number2 = new Base(window, map, { window->screenSize.x / 2 - map->tileSize.x / 2, map->tileSize.y * 4 + map->offset * 2 }, "../resources/images/numbers.png");

	texture = new sf::Texture;
	texture->loadFromFile("../resources/images/player.png");
	textureGun = new sf::Texture;
	textureGun->loadFromFile("../resources/images/guns/gun1.png");
	box = new sf::RectangleShape(sf::Vector2f(map->tileSize.x, map->tileSize.y));

	textureBox = new sf::Texture;
	textureBox->loadFromFile("../resources/images/sslvl" + to_string(lvl) + ".png");
	box->setTexture(textureBox);
	box->setTextureRect(sf::IntRect(0, 16, 16, 16));
	changeBoxPosition();

	body = new sf::RectangleShape;
	body->setTexture(texture);

	bodyGun = new sf::RectangleShape;
	bodyGun->setTexture(textureGun);

	for (int i = 0; i < 3; i++)
		hearts.push_back(new Base(window, map, { 0,0 }, "../resources/images/heart.png"));

	setScreenSize(window->screenSize.x, window->screenSize.y);
}

Player::~Player()
{
	delete body;
	delete texture;
	delete bodyGun;
	delete textureGun;
	delete letter;
	delete number;
	delete number2;
	delete map;
	delete textureBox;
	delete box;

	for (int i = 0; i < enemies.size(); i++)
		delete enemies[i];

	for (int i = 0; i < hearts.size(); i++)
		delete hearts[i];

	for (int i = 0; i < bullets.size(); i++)
		delete bullets[i];

	enemies.clear();
	hearts.clear();
	bullets.clear();
}

void Player::respawn()
{
	respawnBlocks = true;

	position = { startPosition.x, startPosition.y };

	body->setPosition(position.x, position.y);
	bodyGun->setPosition(position.x - map->tileSize.x, position.y);

	if (left)
		bodyGun->setTextureRect(sf::IntRect(48, 0, -48, 16));
	else
		bodyGun->setTextureRect(sf::IntRect(0, 0, 48, 16));

	spawnTimer += 1.f / 60.f;
	visibleTimer += 1.f / 60.f;

	if (visibleTimer >= spawnSwitch / 5)
		visibleTimer = 0,
		visible = !visible;

	if (spawnTimer >= spawnSwitch)
	{
		spawnTimer = 0.f;
		visibleTimer = 0.f;
		spawning = false;
		visible = true;
	}

	canJump = true;
}

void Player::Killed()
{
	killedTimer += 1.f / 60.f;

	body->setFillColor(sf::Color(255, 255, 255, 255 * (1.f - killedTimer * 4.f)));
	bodyGun->setFillColor(sf::Color(255, 255, 255, 255 * (1.f - killedTimer * 4.f)));

	if (killedTimer >= killedSwitch)
		killed = false,
		spawning = true,
		killedTimer = 0.f,
		body->setFillColor(sf::Color(255, 255, 255, 255)),
		bodyGun->setFillColor(sf::Color(255, 255, 255, 255));
}

void Player::draw()
{
	map->drawMap();

	window->draw(*box);

	{
		scoreString = to_string(score);

		for (int i = 0; i < scoreString.size(); i++)
			number->setUVRect(scoreString[i] - 48, 0),
			number->setPosition(window->screenSize.x / 2 + (-float(scoreString.size()) / 2 + i) * number->size.x, map->tileSize.y + +map->offset * 2 + (map->tileSize.y - number->size.y) / 2),
			number->draw();

		scoreString = to_string(maxScore);

		for (int i = 0; i < scoreString.size(); i++)
			number2->setUVRect(scoreString[i] - 48, 0),
			number2->setPosition(window->screenSize.x / 2 + (-float(scoreString.size()) / 2 + i) * number2->size.x, map->tileSize.y * 2 + +map->offset * 2),
			number2->draw();
	}

	for (int i = 0; i < enemies.size(); i++)
		enemies[i]->draw();

	for (int i = 0; i < bullets.size(); i++)
		bullets[i]->draw();

	if (visible && !noHeartsLeft)
		window->draw(*bodyGun),
		window->draw(*body);

	if (noHeartsLeft)
	{
		resetLevel = "PRESS SPACE TO RESTART";
		for (int i = 0; i < resetLevel.size(); i++)
		{
			letter->setColor(255, 255, 255, 255);
			letter->setUVRect(resetLevel[i] - 65, 1);
			letter->setPosition(window->screenSize.x / 2 + (-float(resetLevel.size()) / 2 + i) * letter->size.x, map->offset * 2 + map->tileSize.y * 9 + (map->tileSize.y - letter->size.y) / 2);
			if(resetLevel[i] >= 65)
				letter->draw();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			resetPlayer = true;
	}

	if (displayName)
		displayGunName();

	for (int i = 0; i < hearts.size(); i++)
		hearts[i]->setPosition((i + 1) * map->tileSize.x, window->screenSize.y - map->tileSize.y * 2),
		hearts[i]->draw();

	map->hideOffset();
}

void Player::update()
{
	if (hearts.size() <= 0)
		noHeartsLeft = true;

	if (!noHeartsLeft)
	{
		if (killed)
			Killed();

		if (spawning)
			respawn();

		if (!killed && !spawning)
			checkMovement();

		if (!spawning)
			updateGravity();

		doAttack();
		collision();
		doMovement();
	}

	if (respawnBlocks)
	{
		if (map->returnTile(12, 15) == 0 && map->returnTile(12, 16) == 0)
		{
			map->changeTile(12, 15, 23);
			map->changeTile(12, 16, 24);
			map->changeColTile(12, 15, map->RESPAWN_BLOCK);
			map->changeColTile(12, 16, map->RESPAWN_BLOCK);
		}

		int tempX, tempY;

		tempX = int(position.x / map->tileSize.x);
		tempY = int(position.y / map->tileSize.y);

		if (map->returnTile(12, 15) == 23 && map->returnTile(12, 16) == 24)
		{
			if (tempX < 14 || tempX > 16 || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				respawnBlocks = false;
				map->changeTile(12, 15, 0);
				map->changeTile(12, 16, 0);
				map->changeColTile(12, 15, map->NOTHING);
				map->changeColTile(12, 16, map->NOTHING);
			}
		}
	}

	bulletCollisionEnemies();
	updateTexture();
	updateBullets();
	updateEnemies();

	if (boxAni)
		boxAnimation();

	draw();
}

void Player::updateGravity()
{
	if (speed.y < gravity)
		speed.y += gravityAcc;

	if (speed.x > speedAcc / 2.f)
		speed.x -= speedAcc / 2.f;
	else if (speed.x < -speedAcc / 2.f)
		speed.x += speedAcc / 2.f;
	else
		speed.x = 0.f;
}

void Player::collision()
{
	bool tB = false;

	if (position.y + speed.y < 0 + map->offset)
		speed.y = 0;
	if (position.y > window->screenSize.y - map->offset)
	{
		spawning = true;
		delete hearts[hearts.size() - 1];
		hearts.erase(hearts.begin() + hearts.size() - 1);
	}

	hv::position playerPos = { float(int(position.x / map->tileSize.x)), float(int(position.y / map->tileSize.y)) };

	bool resetY = false;
	bool megaJump = false;

	for (int row = -2; row < 3; row++)
	{
		for (int pos = -1; pos < 3; pos++)
		{
			if (map->returnColMap(playerPos.y + row, playerPos.x + pos) != map->NOTHING)
			{
				hv::bVec3 temp = collisionCheck({ (pos + playerPos.x) * map->tileSize.x, (row + playerPos.y) * map->tileSize.y + map->offset});

				if (temp.x)
					speed.x = 0;

				if (temp.y)
					resetY = true;

				if (temp.z)
					tB = true;

				if (temp.z)
				{
					if (map->returnColMap(playerPos.y + row, playerPos.x + pos) == map->JUMP)
						speed.y = -jumpHeight * 1.2f,
						megaJump = true;

					if (map->returnColMap(playerPos.y + row, playerPos.x + pos) == map->SLOW)
						speed.x /= 2;
				}

				if (temp.x && temp.y)
				{
					hv::position tPosP = position;
					tPosP.add(speed);
					hv::position tPosO = { (pos + playerPos.x) * map->tileSize.x, (row + playerPos.y) * map->tileSize.y + map->offset};

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
	}
	
	hv::bVec3 temp = collisionCheck({ box->getPosition().x, box->getPosition().y });
	if (temp.x || temp.y)
	{
		if (rand() % 2 == 0)
			currentBox = 0;
		else
			currentBox = 16;

		switchWeapon();
		changeBoxPosition();
	}

	if (resetY && !megaJump)
		speed.y = 0;

	for (int e = 0; e < enemies.size(); e++)
	{
		hv::position cX = enemies[e]->position;
		hv::position other = position;

		if (cX.x - map->tileSize.x < other.x &&
			cX.x + enemies[e]->getSize().x > other.x &&
			cX.y - map->tileSize.y < other.y &&
			cX.y + enemies[e]->getSize().y > other.y)
		{
			if (!enemies[e]->dying && !killed && !spawning)
			{
				killed = true;
				if (hearts.size() > 0)
					delete hearts[hearts.size() - 1],
					hearts.erase(hearts.begin() + hearts.size() - 1);
			}
		}
	}

	canJump = tB;
}

hv::bVec3 Player::collisionCheck(hv::position other)
{
	hv::bVec3 bVec = { false, false, false };
	hv::position temp = position;
	temp.add(speed);

	hv::position cX = { temp.x, position.y };
	hv::position cY = { position.x, temp.y };

	if (cX.x - map->tileSize.x < other.x &&
		cX.x + map->tileSize.x > other.x &&
		cX.y - map->tileSize.y < other.y &&
		cX.y + map->tileSize.y > other.y)
		bVec.x = true;

	if (cY.x - map->tileSize.x < other.x &&
		cY.x + map->tileSize.x > other.x &&
		cY.y - map->tileSize.y < other.y &&
		cY.y + map->tileSize.y > other.y)
		bVec.y = true;

	if (cY.x - map->tileSize.x < other.x &&
		cY.x + map->tileSize.x > other.x &&
		cY.y < other.y &&
		cY.y + map->tileSize.y > other.y)
		bVec.z = true;

	return bVec;
}

void Player::checkMovement()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && speed.x > -maxSpeed)
		speed.x -= speedAcc,
		facing = -1.f,
		left = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && speed.x < maxSpeed)
		speed.x += speedAcc,
		facing = 1.f,
		left = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && canJump)
		canJump = false,
		speed.y = -jumpHeight;

	if (gunType == MACHINEGUN || gunType == SHOTGUN)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && canAttack)
			attacking = true;
	}
	else
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !pressedFire)
			attacking = true,
			pressedFire = true;
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && pressedFire)
			pressedFire = false;
	}
}

void Player::doMovement()
{
	position.add(speed);
	body->setPosition(position.x, position.y);
	bodyGun->setPosition(position.x - map->tileSize.x, position.y);

	if (left)
		bodyGun->setTextureRect(sf::IntRect(48, 0, -48, 16));
	else
		bodyGun->setTextureRect(sf::IntRect(0, 0, 48, 16));
}

void Player::doAttack()
{
	if (attacking)
	{
		if (canAttack)
		{
			float aliveTime = 5.f;
			int j = 1;
			if (gunType == SHOTGUN)
				j = 6,
				aliveTime = 0.5f;

			for (int i = 0; i < j; i++)
			{
				hv::position temp(facing * maxSpeed * 2.f, (float(rand() % 400 - 200) / 100.f / 1.5f) * spread * maxSpread);

				bool tLeft = left;

				if (gunType == DUAL_PISTOL)
					tLeft = true,
					temp.x = -1 * maxSpeed * 2.f;;

				if (tLeft)
					bullets.push_back(new Bullet(window, map, { position.x - body->getSize().x * 0.5f, position.y + body->getSize().y * 0.334f }, temp, damage, aliveTime));

				if (gunType == DUAL_PISTOL)
					tLeft = false,
					temp.x = 1 * maxSpeed * 2.f;;

				if (!tLeft)
					bullets.push_back(new Bullet(window, map, { position.x + body->getSize().x * 1.5f, position.y + body->getSize().y * 0.334f }, temp, damage, aliveTime));

				if (left && knockback != 0.f)
					speed.x += knockback / knockbackDevider;
				else if(knockback != 0.f)
					speed.x -= knockback / knockbackDevider;

				canAttack = false;
			}
		}

		if (!canAttack)
		{
			attackTimer += 1.f / 60.f;
			if (attackTimer >= attackSwitch)
			{
				attackTimer = 0;
				canAttack = true;
				attacking = false;
			}
		}
	}
}

void Player::updateBullets()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		bullets[i]->update();
		bullets[i]->collision();
		if (bullets[i]->dead)
			delete bullets[i],
			bullets.erase(bullets.begin() + i);
	}
}

void Player::updateEnemies()
{
	enemySpawnTimer += 1.f / 60.f;

	if (enemySpawnTimer >= enemySpawnSwitch)
	{
		enemySpawnTimer = 0.f;
		enemySpawnSwitch = rand() % 5 + 1;

		int enemyNumber = rand() % 100;

		if (enemyNumber < 70)
			enemyNumber = 0;
		else if (enemyNumber < 90)
			enemyNumber = 1;
		else
			enemyNumber = 2;

		enemies.push_back(new Enemy(window, map, { window->screenSize.x / 2, -map->tileSize.y }, enemyNumber));
	}

	for (int i = 0; i < enemies.size(); i++)
		enemies[i]->update();
}

void Player::updateTexture()
{
	if(speed.x != 0)
		aniTimer += 1.f / 60.f;

	if (aniTimer >= aniSwitch)
	{
		aniTimer = 0.f;
		ani++;

		if (ani > 1)
			ani = 0;
	}

	if (canJump)
		body->setTextureRect(sf::IntRect(ani * 16, int(left) * 16, 16, 16));
	else
		body->setTextureRect(sf::IntRect(int(left) * 16, 32, 16, 16));
}

void Player::switchWeapon()
{
	int rInt = rand() % 5 + 1;

	while(rInt == currentWeapon)
		rInt = rand() % 5 + 1;

	switch (rInt)
	{
	case(1) :
		gunName= "PISTOL";
		gunType = PISTOL;
		attackSwitch = 0.f;
		currentWeapon = 1;
		damage = 0.5f;
		knockback = 2.5f;
		spread = 0.f;
		break;
	case(2) :
		gunName = "MACHINE GUN";
		gunType = MACHINEGUN;
		attackSwitch = 1.f / 14.f;
		currentWeapon = 2;
		damage = 0.5f;
		knockback = 2.5f;
		spread = 1.f;
		break;
	case(3) :
		gunName = "MINIGUN";
		gunType = MACHINEGUN;
		attackSwitch = 1.f / 50.f;
		currentWeapon = 3;
		damage = 0.5f;
		knockback = 2.5f;
		spread = 2.f;
		break;
	case(4) :
		gunName = "SHOTGUN";
 		gunType = SHOTGUN;
		attackSwitch = 1.f / 2.f;
		currentWeapon = 4;
		damage = 0.5f;
		knockback = 2.0f;
		spread = 2.f;
		break;
	case(5) :
		gunName = "DUAL PISTOLS";
		gunType = DUAL_PISTOL;
		attackSwitch = 0.f;
		currentWeapon = 5;
		damage = 0.5f;
		knockback = 0.f;
		spread = 0.f;
		break;
	}

	textureGun->loadFromFile("../resources/images/guns/gun" + to_string(currentWeapon) + ".png");
	maxScore += 30;
	displayName = true;
	displayTimer = 0.f;
}

void Player::bulletCollisionEnemies()
{
	for (int e = 0; e < enemies.size(); e++)
	{
		for (int b = 0; b < bullets.size(); b++)
		{
			hv::position cX = enemies[e]->position;
			hv::position other = bullets[b]->position;

			if (cX.x - bullets[b]->size.x < other.x &&
				cX.x + enemies[e]->getSize().x > other.x &&
				cX.y - bullets[b]->size.y < other.y &&
				cX.y + enemies[e]->getSize().y > other.y &&
				!enemies[e]->dying)
			{
				bullets[b]->dead = true;
				enemies[e]->damage(bullets[b]->damage);
			}
		}

		if (enemies[e]->dead)
		{
			delete enemies[e];
			enemies.erase(enemies.begin() + e);
			if (score < maxScore)
				score += 10,
				kills++;
		}
	}
}

void Player::displayGunName()
{
	displayTimer += 1.f / 60.f;

	if (displayTimer < displaySwitch / 4.f)
		letter->changeAlpha(255 * displayTimer / (displaySwitch / 4.f));


	if (displayTimer > displaySwitch / 4.f * 3.f && displayTimer < displaySwitch)
		letter->changeAlpha(255 * ( 1 - (displayTimer - displaySwitch / 4.f * 3.f) / (displaySwitch / 4.f)));


	int stringLength = gunName.size();
	for (int i = 0; i < stringLength; i++)
	{
		int tempLetter = gunName[i] - 65;
		letter->setUVRect(tempLetter, 1);
		letter->setPosition(position.x + (-float(stringLength) / 2 + i) * letter->size.x + map->tileSize.x / 2, (position.y - map->tileSize.y) + map->offset + (map->tileSize.y - letter->size.y) / 2);
		if (tempLetter >= 0 && tempLetter < 26)
			letter->draw();
	}

	if (displayTimer >= displaySwitch)
		displayTimer = 0.f,
		displayName = false;
}

void Player::setScreenSize(int x, int y)
{
	map->setScreenSize();

	for (int i = 0; i < enemies.size(); i++)
		enemies[i]->setScreenSize();

	for (int i = 0; i < hearts.size(); i++)
		hearts[i]->setScreenSize();

	for (int i = 0; i < bullets.size(); i++)
		bullets[i]->setScreenSize();

	body->setSize(sf::Vector2f(map->tileSize.x, map->tileSize.y));
	bodyGun->setSize(sf::Vector2f(map->tileSize.x * 3, map->tileSize.y));
	box->setSize(sf::Vector2f(map->tileSize.x, map->tileSize.y));
	
	int temp = window->screenSize.x / 1920 * 30;

	letter->setScreenSize(0.5f);
	number->setScreenSize(0.7f);
	number2->setScreenSize(0.5f);
	number2->setColor(125, 125, 125, 255);
}

void Player::changeBoxPosition()
{
	int x, y;

	bool worked = false;

	while (!worked)
	{
		x = rand() % 30; x++;
		y = rand() % 18; y++;

		bool possible = false;
		int lx = x - int(boxPosition.x / map->tileSize.x);
		if (lx < 0)
			lx = -lx;
		int ly = y - int(boxPosition.y / map->tileSize.y);
		if (ly < 0)
			ly = -ly;
		int lz = int(float(x * x) + float(y * y));
		lz = sqrt(float (lz));

		if (lz > 15)
			possible = true;

		if (map->returnColMap(y, x) == 0 && map->returnColMap(y + 1, x) == 1 && possible)
			worked = true;
	}

	boxPositionOld = boxPosition;
	boxPosition = { x * map->tileSize.x, y * map->tileSize.y + map->offset };

	boxAni = true;
}

void Player::boxAnimation()
{
	boxAniTimer += 1.f / 60.f;

	float temp = 1.f / boxAniSwitch;

	box->setSize(sf::Vector2f(map->tileSize.x * (boxAniTimer * temp), map->tileSize.y * (boxAniTimer * temp)));
	box->setPosition(boxPosition.x + (map->tileSize.x - box->getSize().x + box->getSize().x / 2.f) - map->tileSize.x / 2, boxPosition.y + (map->tileSize.y - box->getSize().y));

	if (boxAniTimer >= boxAniSwitch)
		boxAniTimer = 0.f,
		boxAni = false;
}