#include "Includes.h"

#include "Window.h"
#include "TileMap.h"
#include "Menu.h"
#include "Player.h"
#include "musicManager.h"

int main()
{
	Window* window = new Window({ float(sf::VideoMode::getDesktopMode().width), float(sf::VideoMode::getDesktopMode().height) }, "WINDOW NAME");
	Menu* menu = new Menu(window);
	Player* player = NULL;
	musicManager* m_manager = new musicManager();

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
			m_manager->updateSound(menu->volume, menu->playMusic);

			if (!menu->inMenu)
			{
				if (player != NULL)
					delete player,
					player = NULL;

				player = new Player(menu->level, window, { 0.5f, 0.5f });
				m_manager->setLevelMusic(menu->level);
			}
		}
		else
		{
			player->update();

			if(player->resetPlayer)
				delete player,
				player = new Player(menu->level, window, { 0.5f, 0.5f });

			if (player->returnToMenu || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				menu->reset(),
				m_manager->setLevelMusic(0);
		}
		window->display();
		window->clear();
	}

	delete player;
	delete m_manager;
	delete window;
	delete menu;
}