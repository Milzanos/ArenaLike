#include "Includes.h"

#include "Window.h"
#include "TileMap.h"
#include "Menu.h"
#include "Player.h"

int main()
{
	Window* window = new Window({ float(sf::VideoMode::getDesktopMode().width), float(sf::VideoMode::getDesktopMode().height) }, "WINDOW NAME");
	Menu* menu = new Menu(window);
	Player* player = NULL;
	sf::Music* bMusic = new sf::Music;
	bMusic->openFromFile("../resources/sounds/lvl1.wav");
	bMusic->play();
	bMusic->setLoop(true);

	srand(int(time(NULL)));

	while (window->isOpen())
	{
		window->doEvent();
		if (menu->changeScreenSize)
			if (!menu->inMenu)
				player->setScreenSize(window->screenSize.x, window->screenSize.y);

		if (menu->inMenu)
		{
			menu->update();

			if (!menu->inMenu)
			{
				if (player != NULL)
					delete player,
					player = NULL;

				player = new Player(menu->level, window, { 0.5f, 0.5f });
			}
		}
		else
		{
			player->update();

			if(player->resetPlayer)
				delete player,
				player = new Player(menu->level, window, { 0.5f, 0.5f });

			if (player->returnToMenu || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				menu->reset();
		}
		window->display();
		window->clear();
	}

	delete player;
	delete bMusic;
	delete window;
	delete menu;
}