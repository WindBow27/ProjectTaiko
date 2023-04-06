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

enum EndScreenButtonDetail
{
	RESTART = 0,
	TITLE = 1, 
};

class EndScreen
{
public:
	EndScreen(RenderWindow p_window, int p_score, int p_maxcombo, int p_rank, std::vector<int> p_accs);
	void update();
	void render();
	Button* getButton(int num);
private:
	RenderWindow window;
	SDL_Texture* screenTexture;
	SDL_Texture* buttonTexture;
	Mix_Chunk* applause;
	Button* button[2];
	std::vector<SDL_Texture*> rankTextures;
	TTF_Font* font64;
	TTF_Font* font64_outline;
	TTF_Font* font32;
	TTF_Font* font32_outline;
	TTF_Font* font24;
	TTF_Font* font16;
	SDL_Color white = { 255, 255, 255 };
	SDL_Color blue = {1, 215, 251 };
	SDL_Color black = {0, 0, 0};
	SDL_Color red = {234, 20, 18};
	SDL_Color gray = {150,150,150};
	SDL_Color yellow = {255, 214, 38};

	int score;
	int maxcombo;
	int rank;
	std::vector<int>accs;
};