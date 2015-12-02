#include "Includes.h"

#include "Window.h"
#include "TileMap.h"
#include "Menu.h"
#include "Player.h"
#include "musicManager.h"
#include "LevelEditor.h"

int main()
{
	Window* window = new Window({ float(sf::VideoMode::getDesktopMode().width), float(sf::VideoMode::getDesktopMode().height) }, "WINDOW NAME");
	Menu* menu = new Menu(window);
	Player* player = NULL;
	musicManager* m_manager = new musicManager();
	LevelEditor* levelEditor = NULL;
	bool inEditor = false;
	int editor_lvl = 0;


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

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
			{
				menu->inMenu = false;
				inEditor = true;
				editor_lvl = 1;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
			{
				menu->inMenu = false;
				inEditor = true;
				editor_lvl = 2;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
			{
				menu->inMenu = false;
				inEditor = true;
				editor_lvl = 3;
			}

			if (!menu->inMenu)
			{
				if (!inEditor)
				{
					if (player != NULL)
						delete player,
						player = NULL;

					player = new Player(menu->level, window, m_manager, { 0.5f, 0.5f });
					m_manager->setLevelMusic(menu->level);
				}
				else
				{
					levelEditor = new LevelEditor("../resources/images/sslvl" + to_string(editor_lvl), window);
					levelEditor->loadMap("../resources/maps/map" + to_string(editor_lvl) + ".MAP");
				}
			}
		}
		else
		{
			if (!inEditor)
			{
				player->update();

				if(player->resetPlayer)
					delete player,
					player = new Player(menu->level, window, m_manager, { 0.5f, 0.5f });

				if (player->returnToMenu || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					menu->reset(),
					m_manager->setLevelMusic(0);
			}
			else
			{
				levelEditor->update();
				levelEditor->drawMap();

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				{
					menu->reset();
					levelEditor->saveMap();
					inEditor = false;
					m_manager->setLevelMusic(0);
				}
			}
		}
		window->display();
		window->clear();
	}

	delete player;
	delete m_manager;
	delete window;
	delete menu;
}