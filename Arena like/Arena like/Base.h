#pragma once
#include "Includes.h"
#include "Window.h"
#include "TileMap.h"

class Base
{
public:
	Base(Window* window, TileMap* map, hv::position position, string fileName);
	~Base();
	void draw();
	void setPosition(float x, float y);
	void setScreenSize(float specialSize = 1.0f);
	void setUVRect(int x, int y);
	void changeAlpha(int a);
	void setColor(int r, int g, int b, int a);

public:
	hv::size size = { 0,0 };

private:
	Window* window;
	TileMap* map;
	sf::RectangleShape* body;
	sf::Texture* texture;
};

