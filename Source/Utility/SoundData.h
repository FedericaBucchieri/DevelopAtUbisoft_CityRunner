#pragma once
#include "stdafx.h"
#include "MovementStatus.h"

class CSoundData
{
public:
	static char* sound_mainMenu;
	static char* sound_background;
	static char* sound_select;
	static char* sound_run;
	static char* sound_attackKnife;
	static char* sound_attackPunch;
	static char* sound_collectCoin;
	static char* sound_jump;
	static char* sound_doubleJump;
	static char* sound_gameover;
	static char* sound_speedBoost;
	static char* sound_jumpBoost;
	static char* sound_healthUp;
	static char* sound_barrel;
};

struct FSoundData
{
	char* soundName;
	bool looping;
	bool playPreviousOnEnd;
};
