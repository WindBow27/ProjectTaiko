#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Math.hpp"
#include "Note.hpp"
#include "Effect.hpp"
#include "Utils.hpp"
#include "Charts.hpp"
#include "Gameplay.hpp"
#include "TitleScreen.hpp"
#include "EndScreen.hpp"
#include "Manual.hpp"
#include "Selection.hpp"

enum Difficulty {
	EASY_DIF = 0,
	NORMAL_DIF = 1,
	HARD_DIF = 2
};

class Game
{
public:
	Game();
	~Game();
	
	void init();
	void handleEvents();
	void update();
	void render();
	void clean();
	bool running();
	void setRunningState(bool run) {
		isRunning = run;
	}
	int getMouseX() 
	{
		return mouseX;
	}
	int getMouseY() 
	{
		return mouseY;
	}
	int gameState;
private:
	bool isRunning;
	RenderWindow window;
	int mouseX, mouseY;
	Mix_Chunk* backSfx;
	Mix_Chunk* clickSfx;
	const int END_POINT = 723;
	int difficulty;
};