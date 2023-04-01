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
#include "TitleScreen.hpp"

TitleScreen::TitleScreen(RenderWindow p_window) : window(p_window)
{
	screenTexture = window.loadTexture("res/textures/titlescreen/title.png");
	buttonTexture = window.loadTexture("res/textures/button.png");
	font32 = TTF_OpenFont("res/fonts/taikofont.ttf", 32);
	font32_outline = TTF_OpenFont("res/fonts/taikofont.ttf", 32);
	TTF_SetFontOutline(font32_outline, 1);
	button[START] = new Button(window, "Play", font32, black, buttonTexture, Vector2f(820,400));
	button[HTP] = new Button(window, "Manual", font32, black, buttonTexture, Vector2f(820,490));
	button[QUIT] = new Button(window, "Quit", font32, black, buttonTexture, Vector2f(820,580));
}

void TitleScreen::update()
{
	button[START]->update();
	button[HTP]->update();
	button[QUIT]->update();
}

void TitleScreen::render()
{
	window.clear();
	window.render(Vector2f(0, 0), screenTexture);
	button[START]->render();
	button[HTP]->render();
	button[QUIT]->render();
	window.display();
}
