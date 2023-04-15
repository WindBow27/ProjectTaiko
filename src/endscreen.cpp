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
#include "Gameplay.hpp"
#include "Endscreen.hpp"

EndScreen::EndScreen(RenderWindow p_window, int p_score, int p_maxcombo, int p_rank, std::vector<int>p_accs) : window(p_window), score(p_score), maxcombo(p_maxcombo), rank(p_rank), accs(p_accs)
{
	screenTexture = window.loadTexture("res/textures/endscreen/endscreen.png");
	buttonTexture = window.loadTexture("res/textures/button.png");
	applause = Mix_LoadWAV("res/sounds/applause.wav");
	font16 = TTF_OpenFont("res/fonts/taikofont.ttf", 16);
	font24 = TTF_OpenFont("res/fonts/taikofont.ttf", 24);
	font32 = TTF_OpenFont("res/fonts/taikofont.ttf", 32);
	font32_outline = TTF_OpenFont("res/fonts/taikofont.ttf", 32);
	font64 = TTF_OpenFont("res/fonts/taikofont.ttf", 64);
	font64_outline = TTF_OpenFont("res/fonts/taikofont.ttf", 64);
	rankTextures.push_back(window.loadTexture("res/textures/endscreen/ranking-S.png"));
	rankTextures.push_back(window.loadTexture("res/textures/endscreen/ranking-A.png"));
	rankTextures.push_back(window.loadTexture("res/textures/endscreen/ranking-B.png"));
	rankTextures.push_back(window.loadTexture("res/textures/endscreen/ranking-C.png"));
	rankTextures.push_back(window.loadTexture("res/textures/endscreen/ranking-D.png"));
	TTF_SetFontOutline(font32_outline, 1);
	TTF_SetFontOutline(font64_outline, 1);
	button[RESTART] = new Button(window, "Restart", font24, black, buttonTexture, Vector2f(720, 525));
	button[TITLE] = new Button(window, "Title Screen", font24, black, buttonTexture, Vector2f(1000, 525));
	Mix_PlayChannel(-1, applause, 0);
}

EndScreen::~EndScreen()
{
    SDL_DestroyTexture(screenTexture);
    SDL_DestroyTexture(buttonTexture);
    Mix_FreeChunk(applause);
    for (auto i: rankTextures) {
        SDL_DestroyTexture(i);
    }
    TTF_CloseFont(font64);
    TTF_CloseFont(font64_outline);
    TTF_CloseFont(font32);
    TTF_CloseFont(font32_outline);
    TTF_CloseFont(font24);
    TTF_CloseFont(font16);
    for (auto i: button) {
        delete i;
    }
}

void EndScreen::update()
{
	button[RESTART]->update();
	button[TITLE]->update();
}

void EndScreen::render()
{
	window.clear();
	//std::cout << score << " " << accs[0] << " " << accs[1] << " " << accs[2] << std::endl;
	window.render(Vector2f(0, 0), screenTexture);
	button[RESTART]->render();
	button[TITLE]->render();

	window.render(Vector2f(110, 195), getScore(score), font64, blue);

	window.render(Vector2f(90, 310), getScore("Perfect: ", accs[0]), font32_outline, black);
	window.render(Vector2f(90, 310), "Perfect: ", font32, yellow);

	window.render(Vector2f(90, 360), "Okay: ", font32, gray);
	window.render(Vector2f(90, 360), getScore("Okay: ", accs[1]), font32_outline, black);

	window.render(Vector2f(90, 410), "Miss: ", font32, red);
	window.render(Vector2f(90, 410), getScore("Miss: ", accs[2]), font32_outline, black);

	window.render(Vector2f(90, 460), "Max Combo: ", font32, blue);
	window.render(Vector2f(90, 460), getScore("Max Combo: ", maxcombo), font32_outline, black);

	window.render(Vector2f(200, 525), getScore(accs[0], accs[1], accs[2]), font64, white);
	window.render(Vector2f(200, 525), getScore(accs[0], accs[1], accs[2]), font64_outline, black);

	switch (rank)
	{
	case 0:
		window.render(Vector2f(870, 225), rankTextures[0]);
		break;
	case 1:
		window.render(Vector2f(870, 225), rankTextures[1]);
		break;
	case 2:
		window.render(Vector2f(870, 225), rankTextures[2]);
		break;
	case 3:
		window.render(Vector2f(870, 225), rankTextures[3]);
		break;
	case 4:
		window.render(Vector2f(870, 225), rankTextures[4]);
		break;
	default:
		window.render(Vector2f(870, 225), rankTextures[4]);
		break;
	}

	window.display();
}

Button* EndScreen::getButton(int num)
{
	return button[num];
}