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

enum SelectionButtonDetail
{
	EASY = 0,
	NORMAL = 1, 
	HARD = 2,
	BACK = 3,

	PRECISE = 0,
	SUDDENDEATH = 1,
	ALLPERFECT = 2,
	BINDINGLIGHT = 3
};

class Selection
{
public:
	Selection(RenderWindow p_window);
	~Selection();
	void init();
	void update();
	void render();
	Button* getButton(int num);
	std::vector<bool> getMod();
	void setRank(int i, SDL_Texture* rankTexture);
private:
	RenderWindow window;
	SDL_Texture* screenTexture;
	SDL_Texture* buttonTexture;
	SDL_Texture* buttonEasyTexture;
	SDL_Texture* buttonNormalTexture;
	SDL_Texture* buttonHardTexture;
	SDL_Texture* preciseTexture;
	SDL_Texture* suddendeathTexture;
	SDL_Texture* allperfectTexture;
	SDL_Texture* bindinglightTexture;
	SDL_Texture* precise1Texture;
	SDL_Texture* suddendeath1Texture;
	SDL_Texture* allperfect1Texture;
	SDL_Texture* bindinglight1Texture;
	SDL_Texture* rankS;
	SDL_Texture* rankA;
	SDL_Texture* rankB;
	SDL_Texture* rankC;
	SDL_Texture* rankD;
	SDL_Texture* descBinding[4];
	SDL_Texture* descButton[3];
	Button* button[4];
	Button* binding[4];
	std::vector<bool> mod = {0, 0, 0, 0};
	std::vector<int> highscore;
	TTF_Font* font32;
	TTF_Font* font40;
	TTF_Font* font32_outline;
	SDL_Color black = { 0, 0, 0 };
};
