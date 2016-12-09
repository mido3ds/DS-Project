#pragma once
#include <windows.h>

// to play sounds, make this macro = 1
//#define __PLAY_SND_ 

#ifndef __PLAY_SND_
	#define Play(name) 
	#define Playloop(name) 
	#define Stop() 
#else
	#define Play(name) PlaySound(TEXT(name), NULL, SND_FILENAME | SND_ASYNC)
	#define Playloop(name) PlaySound(TEXT(name), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP)
	#define Stop() PlaySound(NULL, NULL, NULL) 
#endif //__PLAY_SND_


/* sounds */

#define INTRO_SOUND ".\\sounds\\intro.wav"
#define LASER_SOUND ""
#define WIN_SOUND ".\\sounds\\win.wav"
#define GAME_OVER_SOUND ""
#define FIGHT_SOUND ".\\sounds\\fight.wav"
#define GUN_SOUND ".\\sounds\\gun.wav"
#define MENU_SOUND ".\\sounds\\menu.wav"