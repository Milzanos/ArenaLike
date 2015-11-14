#pragma once
#include "Includes.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Window.h"
#include "TileMap.h"
#include "Base.h"

class Player
{
public:
	Player(int lvl, Window* window, hv::position position);
	~Player();

	void update();
	void draw();
	void setScreenSize(int x, int y);

private:
	void respawn();
	void updateGravity();
	void collision();
	void checkMovement();
	hv::bVec3 collisionCheck(hv::position other);
	void doMovement();
	void doAttack();
	void bulletCollisionEnemies();
	void updateBullets();
	void updateEnemies();
	void updateTexture();
	void switchWeapon();
	void displayGunName();
	void Killed();
	void changeBoxPosition();
	void boxAnimation();
	
public:
	bool returnToMenu = false;
	bool resetPlayer = false;

private:
	TileMap* map = NULL;
	Window* window = NULL;

	hv::position position = { 0.f,0.f };
	hv::position startPosition = { 0.f,0.f };

	sf::Texture* texture = NULL;
	sf::RectangleShape* body = NULL;
	sf::Texture* textureGun = NULL;
	sf::RectangleShape* bodyGun = NULL;
	sf::Texture* textureBox = NULL;
	sf::RectangleShape* box = NULL;

	bool boxAni = false;
	float boxAniTimer = 0.0f;
	float boxAniSwitch = 1.f;
	int currentBox = 0;
	hv::position boxPosition = { 0.f,0.f };
	hv::position boxPositionOld = { 0.f, 0.f };

	float aniTimer = 0.f;
	float aniSwitch = 0.25f;
	int ani = 0;
	bool left = false;

	bool spawning = false;
	float spawnTimer = 0.f;
	float spawnSwitch = 2.f;
	bool visible = true;
	float visibleTimer = 0.f;

	bool killed = false;
	float killedTimer = 0.f;
	float killedSwitch = 0.25f;

	hv::position speed = { 0.f, 0.f };

	bool respawnBlocks = true;

	float maxSpeed = 6.f;
	float speedAcc = 40.f;
	float gravity = 5.f;
	float gravityAcc = 60.f;
	float jumpHeight = 2.4f;

	float facing = 1.f;

	bool canJump = false;
	bool pressedFire = true;
	int gunType = 0;

	enum
	{
		PISTOL,
		DUAL_PISTOL,
		BAZOOKA,
		FLAMETHROWER,
		MACHINEGUN,
		SHOTGUN
	};

	int currentWeapon = 1;
	float knockback = 2.5f;
	float knockbackDevider = 60.f;

	string gunName;
	bool displayName = false;
	float displayTimer = 0.f;
	float displaySwitch = 2.f;

	int score = 0;
	string scoreString;
	int maxScore = 30;
	int kills = 0;

	Base* letter;
	Base* number;
	Base* number2;

	vector<Enemy*> enemies;
	vector<Base*> hearts;
	vector<Bullet*> bullets;

	sf::Music* s_bullet = NULL;
	
	string resetLevel = "PRESS SPACE TO RESTART";
	bool noHeartsLeft = false;

	float damage = 0.5f;
	float spread = 0.f;
	float maxSpread = 60.f;

	float enemySpawnTimer = 0.f;
	float enemySpawnSwitch = 1.0f;

	bool attacking = false;
	bool canAttack = true;
	float attackTimer = 0.f;
	float attackSwitch = 0.f;
};

