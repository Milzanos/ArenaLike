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
	bool playMusic = true;
	int volume = 7;

private:
	Window* window;
	TileMap* mainMap;
	TileMap* optionsMap;
	Base* number;
	Base* letter;
	Base* number2;
	Base* letter2;
	string gameName = "ARENA BOXES";
	
	vector<string> vMainMenu;
	vector<string> vLevelSelect;
	vector<string> vOptions;

	string resolutionNumber;
	string fullScreenState;
	string soundState;
	string volumeState;

	int selectedItem = 0;
	bool pressedUp = false;
	bool pressedDown = false;
	bool pressedSpace = false;
	bool inOptions = false;
	bool inLevelSelect = false;
	int currentResolution = -1;
	vector<hv::size> resolutions;
};

