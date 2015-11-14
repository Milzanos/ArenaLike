#pragma once
#include "Includes.h"
#include "Window.h"
#include "TileMap.h"

class Enemy
{
public:
	Enemy(Window* window, TileMap* map, hv::position position, int type = 0);
	~Enemy();
	void update();
	void draw();
	void damage(float dmg);
	void setScreenSize();
	hv::size getSize();

private:
	void collision();
	hv::bVec3 collisionCheck(hv::position other);
	void checkMovement();
	void doMovement();
	void updateTexture();

public:
	bool dead = false;
	bool dying = false;
	hv::position position = { 0.f,0.f };

private:
	int ani;
	float aniTimer = 0.f;
	float aniSwitch = 0.25f;
	float health = 1.f;
	int colChecks = 0;
	int enemyUVSize = 0;

	bool left = false;
	bool normal = true;

	float dyingTimer = 0.2f;

	float damageTimer = 0.1f;
	float damageSwitch = 0.1f;

	float maxSpeed = 0.f;
	const float baseMaxSpeed = 12.f;
	float speedAcc = 0.f;
	const float baseSpeedAcc = 40.f;
	float gravity = 0.f;
	const float baseGravity = 5.f;
	float gravityAcc = 0.f;
	const float baseGravityAcc = 60.f;
	float jumpHeight = 0.f;
	const float baseJumpHeight = 2.4f;
	
	Window* window;
	TileMap* map;
	sf::Texture* texture;
	sf::RectangleShape* body;
	hv::position speed = { 0.f,0.f };
};

