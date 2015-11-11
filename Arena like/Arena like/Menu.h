#pragma once
#include "Includes.h"
#include "Window.h"
#include "TileMap.h"
#include "Base.h"

class Menu
{
public:
	Menu(Window* window);
	~Menu();
	void update();
	void reset();

private:
	void draw();
	void checkKeyboardInput();
	void menuOptions();
	void drawString(string& str, Base* base, int type, int item, int height);
	void drawString(string& str, Base* base, int type, int height);

public:
	bool inMenu = true;
	bool changeScreenSize = true;
	int level = 1;

private:
	Window* window;
	TileMap* mainMap;
	TileMap* optionsMap;
	Base* number;
	Base* letter;
	Base* number2;
	Base* letter2;
	string gameName = "ARENA BOXES";
	
	string StartGame = "START GAME";
	string Options = "OPTIONS";
	string Exit = "EXIT";

	string Level1 = "LEVEL ONE";
	string Level2 = "LEVEL TWO";
	string Level3 = "LEVEL THREE";
	string LevelBack = "BACK";
	
	string resolution = "RESOLUTION";
	string resolutionNumber;
	string fullScreen = "FULL SCREEN";
	string fullScreenState;
	string sound = "SOUND";
	string soundState;
	string back = "RETURN AND ACCEPT";

	int selectedItem = 0;
	bool pressedUp = false;
	bool pressedDown = false;
	bool pressedSpace = false;
	bool inOptions = false;
	bool inLevelSelect = false;
	bool playSound = true;
	int currentResolution = -1;
	vector<hv::size> resolutions;
};

