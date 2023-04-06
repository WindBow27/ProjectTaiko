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
#include "Manual.hpp"

Manual::Manual(RenderWindow p_window) : window(p_window)
{
	screenTexture = window.loadTexture("res/textures/manual/manual.png");
	buttonTexture = window.loadTexture("res/textures/button.png");
	font32 = TTF_OpenFont("res/fonts/taikofont.ttf", 32);
	font32_outline = TTF_OpenFont("res/fonts/taikofont.ttf", 32);
	TTF_SetFontOutline(font32_outline, 1);
	button = new Button(window, "Got it!", font32, black, buttonTexture, Vector2f(512.5, 550));
}

void Manual::update()
{
	button->update();
}

void Manual::render()
{
	window.clear();
	window.render(Vector2f(0, 0), screenTexture);
	button->render();
	window.display();
}

Button* Manual::getButton()
{
	return button;
}