#pragma once
#include "Includes.h"

class musicManager
{
public:
	musicManager();
	~musicManager();
	void updateSound(int volume, bool state);
	void setLevelMusic(int level);

private:
	bool playMusic = false;
	sf::Music* music;
};