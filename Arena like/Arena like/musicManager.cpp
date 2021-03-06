#include "musicManager.h"



musicManager::musicManager()
{
	music = new sf::Music();
	music->openFromFile("../resources/sounds/lvl0.wav");
	music->setLoop(true);
	music->pause();
}

musicManager::~musicManager()
{
	delete music;
}

void musicManager::updateSound(int volume, bool state)
{
	this->volume = volume * 10;
	playMusic = state;
	if (playMusic)
		if (music->Paused == music->getStatus())
			music->play();
	if (!playMusic)
		if (music->Playing == music->getStatus())
			music->stop();

	music->pause();
	music->setVolume(float(volume * 10));
	music->play();
}

void musicManager::setLevelMusic(int level)
{
	music->openFromFile("../resources/sounds/lvl" + to_string(level) + ".wav");

	if(playMusic)
		music->play();
	else
		music->pause();
}