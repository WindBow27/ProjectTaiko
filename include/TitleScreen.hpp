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
	HTP = 1, 
	QUIT = 2,
};

class TitleScreen
{
public:
	TitleScreen(RenderWindow p_window);
	void update();
	void render();
private:
	RenderWindow window;
	SDL_Texture* screenTexture;
	SDL_Texture* buttonTexture;
	Button* button[3];
	TTF_Font* font32;
	TTF_Font* font32_outline;
	SDL_Color black = { 0, 0, 0 };
};