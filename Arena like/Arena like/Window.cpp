#include "Window.h"



Window::Window(hv::size screenSize, string screenName)
{
	windowName = screenName;
	window = new sf::RenderWindow(sf::VideoMode(int(screenSize.x), int(screenSize.y)), screenName, sf::Style::Fullscreen);
	window->setFramerateLimit(60);
	this->screenSize = screenSize;
	this->lastScreenSize = screenSize;
}


Window::~Window()
{
	delete window;
	window = NULL;
}

void Window::doEvent()
{
	sf::Event e;

	while (window->pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
			window->close();
	}
}

//DRAW STUFF
void Window::clear()
{
	window->clear();
}

void Window::display()
{
	window->display();
}

void Window::draw(sf::RectangleShape& other)
{
	window->draw(other);
}

void Window::draw(sf::Text& other)
{
	window->draw(other);
}

void Window::draw(sf::Sprite& other)
{
	window->draw(other);
}

bool Window::isOpen()
{
	return window->isOpen();
}

void Window::setScreenSize(int x, int y)
{
	if (x != lastScreenSize.x || y != lastScreenSize.y || fullScreen != realFullScreen)
	{
		if(fullScreen)
			window->create(sf::VideoMode(x, y), windowName, sf::Style::Fullscreen);
		else
			window->create(sf::VideoMode(x, y), windowName);
		window->setFramerateLimit(60);
		screenSize = { float(x), float(y) };
		lastScreenSize = { float(x), float(y) };
		realFullScreen = fullScreen;
	}
}

void Window::toggleFullScreen()
{
	fullScreen = !fullScreen;
	if (fullScreen)
		window->create(sf::VideoMode(screenSize.x, screenSize.y), windowName, sf::Style::Fullscreen);
	else
		window->create(sf::VideoMode(screenSize.x, screenSize.y), windowName);

	realFullScreen = fullScreen;

	window->setFramerateLimit(60);
}

void Window::close()
{
	window->close();
}

bool Window::getFullScreen()
{
	return realFullScreen;
}

hv::position Window::getMousePosition()
{
	sf::Vector2i temp;

	temp = sf::Mouse::getPosition(*window);
	return hv::position(int(temp.x), int(temp.y));
}