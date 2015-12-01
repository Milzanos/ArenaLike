#pragma once
#include "Includes.h"
#include "Window.h"

class LevelEditor
{
public:
	LevelEditor(string fileName, Window* window);
	~LevelEditor();
	void drawMap();
	void loadMap(string fileName);
	void update();
	void saveMap();

private:
	void setScreenSize();
	void hideOffset();
	bool blockSelection(hv::position mousePos);
	int returnTile(int y, int x);

public:
	hv::size tileSize{ 0.f,0.f };
	hv::size offsetSelection{ 0.f,0.f };
	float offset = 0.f;
	int selectedBlock = 0;

private:
	sf::RectangleShape* sprite = NULL;
	int spritesPerRow = 18;
	sf::RectangleShape* spriteBackground = NULL;
	sf::RectangleShape* offsetHider = NULL;
	sf::Texture* texture = NULL;
	sf::Texture* textureBackground = NULL;
	int map[20][32];
	int background[20][32];
	Window* window = NULL;
	string fileName;
};

