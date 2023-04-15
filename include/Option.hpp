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

class Option
{
public:
	Option(RenderWindow p_window);
	void update();
	void render();
	Button* getButton();
private:
	RenderWindow window;
	SDL_Texture* screenTexture;
	SDL_Texture* buttonTexture;
	SDL_Texture* sliderTexture;
	SDL_Texture* sliderButtonTexture;
	Button* button;
	TTF_Font* font32;
	TTF_Font* font32_outline;
	SDL_Color black = { 0, 0, 0 };
};
