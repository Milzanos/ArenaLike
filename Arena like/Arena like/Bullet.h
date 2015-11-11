#pragma once
#include "Includes.h"
#include "Window.h"
#include "TileMap.h"

class Bullet
{
public:
	Bullet(Window* window, TileMap* map, hv::position position, hv::position speed, float damage, float timeLeft = 1000);
	~Bullet();

	void draw();
	void update();
	void collision();
	void setScreenSize();

private:
	bool Bullet::collisionCheck(hv::position other);

public:
	bool dead = false;
	hv::position position = { 0,0 };
	hv::size size = { 0,0 };
	float damage = 0.f;

private:
	int timesSmaller = 4;
	float timeLeft = 0.f;

	Window* window;
	TileMap* map;
	sf::Texture* texture;
	sf::RectangleShape* body;
	hv::position speed = { 0,0 };
};

