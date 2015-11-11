#include "Menu.h"



Menu::Menu(Window* window)
{
	this->window = window;
	mainMap = new TileMap("../resources/images/menu", this->window);
	mainMap->loadMap("../resources/maps/menu.MAP");
	
	number = new Base(window, mainMap, { 0,0 }, "../resources/images/numbers.png");
	letter = new Base(window, mainMap, { 0,0 }, "../resources/images/letters.png");
	number2 = new Base(window, mainMap, { 0,0 }, "../resources/images/numbers.png");
	letter2 = new Base(window, mainMap, { 0,0 }, "../resources/images/letters.png");

	number2->setScreenSize(0.5f);
	letter2->setScreenSize(0.5f);

	resolutions.push_back(hv::size(800, 600));
	resolutions.push_back(hv::size(1920, 1080));
	resolutions.push_back(hv::size(1280, 720));
	resolutions.push_back(hv::size(1360, 768));
	resolutions.push_back(hv::size(1920, 1200));

	hv::size temp = window->screenSize;

	for (int i = 0; i < resolutions.size(); i++)
	{
		if (temp.x == resolutions[i].x && temp.y == resolutions[i].y)
		{
			currentResolution = i;
		}
	}

	if (currentResolution == -1)
	{
		resolutions.push_back(temp);
		currentResolution = resolutions.size() - 1;
	}
}

Menu::~Menu()
{
	delete mainMap;
	delete number;
	delete letter;
}

void Menu::update()
{
	checkKeyboardInput();
	
	draw();
}

void Menu::draw()
{
	mainMap->drawMap();

	/*for (int i = 0; i < screenResolutionString.size(); i++)
	{
		number->setUVRect(screenResolutionString[i] - 48, 0);
		number->setPosition(window->screenSize.x / 2 + (-float(screenResolutionString.size()) / 2 + i) * number->size.x, mainMap->offset * 2 + mainMap->tileSize.y * 10 );
		if(screenResolutionString[i] >= 48)
			number->draw();
	}*/

	menuOptions();
}

void Menu::checkKeyboardInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !pressedUp)
	{
		pressedUp = true;

		selectedItem--;
		if (!inOptions && !inLevelSelect)
			if (selectedItem < 0)
				selectedItem = 2;
		if (inOptions || inLevelSelect)
			if (selectedItem < 0)
				selectedItem = 3;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !pressedDown)
	{
		pressedDown = true;

		selectedItem++;
		if(!inOptions && !inLevelSelect)
			if (selectedItem > 2)
				selectedItem = 0;
		if(inOptions || inLevelSelect)
			if (selectedItem > 3)
				selectedItem = 0;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !pressedSpace)
	{
		pressedSpace = true;

		if (!inOptions && !inLevelSelect)
		{
			if (selectedItem == 0)
				inLevelSelect = !inLevelSelect;
			else if (selectedItem == 1)
				inOptions = !inOptions,
				selectedItem = 0;
			else if (selectedItem == 2)
				window->close();
		}
		else if(inOptions)
		{
			if (selectedItem == 0)
			{
				currentResolution++;
				if (currentResolution > resolutions.size() - 1)
					currentResolution = 0;
			}
			else if (selectedItem == 1)
				playSound = !playSound;
			else if (selectedItem == 2)
				window->fullScreen = !window->fullScreen;
			else if (selectedItem == 3)
				inOptions = !inOptions,
				selectedItem = 0,
				changeScreenSize = true;
		}
		else if (inLevelSelect)
		{
			if (selectedItem == 0)
				inMenu = false,
				level = 1;
			else if (selectedItem == 1)
				inMenu = false,
				level = 2;
			else if (selectedItem == 2)
				inMenu = false,
				level = 3;
			else if (selectedItem == 3)
				inLevelSelect = !inLevelSelect,
				selectedItem = 0;
		}
	}

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && pressedUp)
		pressedUp = false;

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::S) && pressedDown)
		pressedDown = false;

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && pressedSpace)
		pressedSpace = false;

	if (changeScreenSize)
	{
		window->setScreenSize(int(resolutions[currentResolution].x), int(resolutions[currentResolution].y));
		mainMap->setScreenSize();
		number->setScreenSize();
		letter->setScreenSize();
		number2->setScreenSize(0.5f);
		letter2->setScreenSize(0.5f);

		changeScreenSize = false;
	}
}

void Menu::menuOptions()
{
	drawString(gameName, letter, 1, -1, 5);

	if (!inOptions && !inLevelSelect)
	{
		drawString(StartGame, letter, 1, 0, 7);

		drawString(Options, letter, 1, 1, 9);

		drawString(Exit, letter, 1, 2, 11);
	}
	else if(inOptions)
	{
		drawString(resolution, letter, 1, 0, 7);
		
		resolutionNumber = to_string(int(resolutions[currentResolution].x)) + " " + to_string(int(resolutions[currentResolution].y));

		drawString(resolutionNumber, number2, 0, 8);

		drawString(sound, letter, 1, 1, 9);

		if (playSound)
			soundState = "TRUE";
		else
			soundState = "FALSE";

		drawString(soundState, letter2, 1, 10);

		drawString(fullScreen, letter, 1, 2, 11);

		if (window->fullScreen)
			fullScreenState = "TRUE";
		else
			fullScreenState = "FALSE";

		drawString(fullScreenState, letter2, 1, 12);

		drawString(back, letter, 1, 3, 13);
	}
	else if (inLevelSelect)
	{
		drawString(Level1, letter, 1, 0, 7);

		drawString(Level2, letter, 1, 1, 9);

		drawString(Level3, letter, 1, 2, 11);

		drawString(LevelBack, letter, 1, 3, 13);
	}
}

void Menu::drawString(string& str, Base* base, int type, int item, int height)
{
	if (selectedItem == item)
		base->setColor(125, 0, 0, 255);
	else if (item == -1)
		base->setColor(255, 0, 0, 255);
	else
		base->setColor(255, 255, 255, 255);

	int temp;
	if (type == 0)
		temp = 48;
	else
		temp = 65;

	for (int i = 0; i < str.size(); i++)
	{
		base->setUVRect(str[i] - temp, type);
		base->setPosition(window->screenSize.x / 2 + (-float(str.size()) / 2 + i) * base->size.x, mainMap->offset * 2 + mainMap->tileSize.y * height);
		if (str[i] >= temp)
			base->draw();
	}
}

void Menu::drawString(string& str, Base* base, int type, int height)
{
	base->setColor(125, 125, 125, 255);

	int temp;
	if (type == 0)
		temp = 48;
	else
		temp = 65;

	for (int i = 0; i < str.size(); i++)
	{
		base->setUVRect(str[i] - temp, type);
		base->setPosition(window->screenSize.x / 2 + (-float(str.size()) / 2 + i) * base->size.x, mainMap->offset * 2 + mainMap->tileSize.y * height + (mainMap->tileSize.y - base->size.y) / 2);
		if (str[i] >= temp)
			base->draw();
	}
}

void Menu::reset()
{
	selectedItem = 0;
	inMenu = true;
	inOptions = false;
	inLevelSelect = false;
}