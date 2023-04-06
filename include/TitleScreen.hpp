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
#include "Button.hpp"

enum TitleScreenButtonDetail
{
	START = 0,
	MANUAL = 1, 
	OPTION = 2,
	QUIT = 3,
};

class TitleScreen
{
public:
	TitleScreen(RenderWindow p_window);
	void init();
	void update();
	void render();
	Button* getButton(int num);
private:
	RenderWindow window;
	SDL_Texture* screenTexture;
	SDL_Texture* titleTexture;
	SDL_Texture* buttonTexture;
	Mix_Music* titleMusic;
	Button* button[4];
	TTF_Font* font32;
	TTF_Font* font32_outline;
	SDL_Color black = { 0, 0, 0 };
};
