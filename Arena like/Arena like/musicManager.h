#pragma once
#include "Includes.h"

class musicManager
{
public:
	musicManager();
	~musicManager();
	void updateSound(int volume, bool state);
	void setLevelMusic(int level);

public:
	int volume = 0;
	bool playMusic = false;
private:
	sf::Music* music;
};