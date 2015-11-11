#pragma once
#include "Includes.h"
#include "Window.h"

class TileMap
{
public:
	TileMap(string fileName, Window* window);
	~TileMap();

	int returnTile(int y, int x);
	int returnColMap(int y, int x);
	void changeTile(int y, int x, int type);
	void changeColTile(int y, int x, int type);
	void drawMap();
	void loadMap(string fileName);
	void setScreenSize();
	void hideOffset();

private:

public:
	hv::size tileSize{ 0.f,0.f };
	float offset = 0.f;

	enum
	{
		NOTHING,
		WALL,
		BOX,
		JUMP,
		SLOW,
		RESPAWN_BLOCK
	};

private:
	sf::RectangleShape* sprite = NULL ;
	int spritesPerRow = 18;
	sf::RectangleShape* spriteBackground = NULL ;
	sf::RectangleShape* offsetHider = NULL;
	sf::Texture* texture = NULL;
	sf::Texture* textureBackground = NULL;
	int map[20][32];
	int background[20][32];
	int colMap[20][32];
	Window* window = NULL;
};