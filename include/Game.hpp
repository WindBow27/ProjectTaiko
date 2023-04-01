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
};