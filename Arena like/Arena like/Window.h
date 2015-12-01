#pragma once
#include "Includes.h"

class Window
{
public:
	Window(hv::size screenSize, string screenName);
	~Window();

	void doEvent();
	void clear();
	void display();
	void draw(sf::RectangleShape& other);
	void draw(sf::Sprite& other);
	void draw(sf::Text& other);
	bool isOpen();
	void setScreenSize(int x, int y);
	void toggleFullScreen();
	void close();
	bool getFullScreen();

public:
	hv::size screenSize{ 0,0 };
	bool fullScreen = true;

private:
	bool realFullScreen = true;
	hv::size lastScreenSize{ 0,0 };
	sf::RenderWindow* window;
	string windowName;
};

