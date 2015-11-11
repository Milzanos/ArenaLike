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

	vOptions.push_back("RESOLUTION");
	vOptions.push_back("FULL SCREEN");
	vOptions.push_back("SOUND");
	vOptions.push_back("VOLUME");
	vOptions.push_back("RETURN AND ACCEPT");

	vMainMenu.push_back("START GAME");
	vMainMenu.push_back("OPTIONS");
	vMainMenu.push_back("EXIT");

	vLevelSelect.push_back("LEVEL ONE");
	vLevelSelect.push_back("LEVEL TWO");
	vLevelSelect.push_back("LEVEL THREE");
	vLevelSelect.push_back("BACK");

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
				selectedItem = vMainMenu.size() - 1;
		if (inOptions)
			if (selectedItem < 0)
				selectedItem = vOptions.size() - 1;
		if(inLevelSelect)
			if (selectedItem < 0)
				selectedItem = vLevelSelect.size() - 1;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !pressedDown)
	{
		pressedDown = true;

		selectedItem++;
		if(!inOptions && !inLevelSelect)
			if (selectedItem > vMainMenu.size() - 1)
				selectedItem = 0;
		if(inOptions)
			if (selectedItem > vOptions.size() - 1)
				selectedItem = 0;
		if (inLevelSelect)
			if (selectedItem > vLevelSelect.size() - 1)
				selectedItem = 0;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !pressedSpace)
	{
		pressedSpace = true;

		if (!inOptions && !inLevelSelect)
		{
			switch (selectedItem)
			{
			case(0) :
				inLevelSelect = !inLevelSelect;
				break;
			case(1) :
				inOptions = !inOptions;
				selectedItem = 0;
				break;
			case(2) :
				window->close();
				break;
			}
		}
		else if(inOptions)
		{
			switch (selectedItem)
			{
			case(0):
				currentResolution++;
				if (currentResolution > resolutions.size() - 1)
					currentResolution = 0;
				break;
			case(1) :
				window->fullScreen = !window->fullScreen;
				break;
			case(2) :
				playMusic = !playMusic;
				break;
			case(3):
				volume++;
				if (volume > 10)
					volume = 0;
				break;
			case(4):
				inOptions = !inOptions,
				selectedItem = 0,
				changeScreenSize = true;
				break;
			}
				
		}
		else if (inLevelSelect)
		{
			switch (selectedItem)
			{
			case(0) :
				inMenu = false;
				level = 1;
				break;
			case(1) :
				inMenu = false;
				level = 2;
				break;
			case(2) :
				inMenu = false;
				level = 3;
				break;
			case(3) :
				inLevelSelect = !inLevelSelect;
				selectedItem = 0;
				break;
			}
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
		for (int i = 0; i < vMainMenu.size(); i++)
			drawString(vMainMenu[i], letter, 1, i, 7 + i * 2);
	}
	else if(inOptions)
	{
		for (int i = 0; i < vOptions.size(); i++)
			drawString(vOptions[i], letter, 1, i, 7 + i * 2);
		
		//RESOLUTION
		resolutionNumber = to_string(int(resolutions[currentResolution].x)) + " " + to_string(int(resolutions[currentResolution].y));

		drawString(resolutionNumber, number2, 0, 8);

		//FULLSCREEN
		if (window->fullScreen)
			fullScreenState = "TRUE";
		else
			fullScreenState = "FALSE";

		drawString(fullScreenState, letter2, 1, 10);

		//PLAY MUSIC
		if (playMusic)
			soundState = "TRUE";
		else
			soundState = "FALSE";

		drawString(soundState, letter2, 1, 12);

		//VOLUME
		volumeState = to_string(volume * 10);
		drawString(volumeState, number2, 0, 14);
	}
	else if (inLevelSelect)
	{
		for (int i = 0; i < vLevelSelect.size(); i++)
			drawString(vLevelSelect[i], letter, 1, i, 7 + i * 2);
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