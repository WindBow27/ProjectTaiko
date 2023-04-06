#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cmath>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Math.hpp"
#include "Note.hpp"
#include "Effect.hpp"
#include "Utils.hpp"
#include "TitleScreen.hpp"

TitleScreen::TitleScreen(RenderWindow p_window) : window(p_window)
{
	screenTexture = window.loadTexture("res/textures/titlescreen/title.png");
	buttonTexture = window.loadTexture("res/textures/button.png");
	titleTexture = window.loadTexture("res/textures/titlescreen/game-title-2.png");
	font32 = TTF_OpenFont("res/fonts/taikofont.ttf", 32);
	font32_outline = TTF_OpenFont("res/fonts/taikofont.ttf", 32);
	TTF_SetFontOutline(font32_outline, 1);
	button[START] = new Button(window, "Play", font32, black, buttonTexture, Vector2f(620,465));
	button[MANUAL] = new Button(window, "Tutorial", font32, black, buttonTexture, Vector2f(620,575));
	button[OPTION] = new Button(window, "Options", font32, black, buttonTexture, Vector2f(930,465));
	button[QUIT] = new Button(window, "Quit", font32, black, buttonTexture, Vector2f(930,575));
	titleMusic = Mix_LoadMUS("res/sounds/target-for-love.mp3");
}

void TitleScreen::init()
{
	Mix_PlayMusic(titleMusic, -1);
}

void TitleScreen::update()
{
	button[START]->update();
	button[MANUAL]->update();
	button[OPTION]->update();
	button[QUIT]->update();
}

void TitleScreen::render()
{
	window.clear();
	window.render(Vector2f(0, 0), screenTexture);
	window.render(Vector2f(0, sin(SDL_GetTicks()/200) * 2 - 4), titleTexture);
	button[START]->render();
	button[MANUAL]->render();
	button[OPTION]->render();
	button[QUIT]->render();
	window.display();
}

Button* TitleScreen::getButton(int num)
{
	return button[num];
}

