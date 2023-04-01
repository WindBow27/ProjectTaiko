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

Gameplay::Gameplay(RenderWindow p_window) : window(p_window)
{

}

void Gameplay::init()
{
	donSfx = Mix_LoadWAV("res/sounds/taiko-normal-hitnormal.wav");
	kaSfx = Mix_LoadWAV("res/sounds/taiko-normal-hitclap.wav");
	missSfx = Mix_LoadWAV("res/sounds/taiko-normal-hitwhistle.wav");
	reAoharuMusic = Mix_LoadMUS("res/sounds/reaoharu-audio.mp3");

	donTexture = window.loadTexture("res/textures/don.png");
	kaTexture = window.loadTexture("res/textures/ka.png");

	slider.push_back(Entity(Vector2f(0, PLACEMENT), window.loadTexture("res/textures/slider/taiko-bar-left.png")));
	slider.push_back(Entity(Vector2f(190, PLACEMENT), window.loadTexture("res/textures/slider/taiko-bar-middle.png")));
	slider.push_back(Entity(Vector2f(390, PLACEMENT), window.loadTexture("res/textures/slider/taiko-bar-right.png")));
	
	//Background textures
    for (int i = 1; i <= 723; i++) {
        std::vector<const char*> v = getText("res/textures/background/", i, ".png");
        bgfile.insert(bgfile.end(), v.begin(), v.end());
    }
    for (auto i: bgfile) backgrounds.push_back(Entity(Vector2f(0,0), window.loadTexture(i)));
    for (auto i : bgfile) delete[] i; // Free memory for const char*

    shiroko.push_back(Entity(Vector2f(0, 18), window.loadTexture("res/textures/shiroko/0.png")));
	shiroko.push_back(Entity(Vector2f(0, 18), window.loadTexture("res/textures/shiroko/1.png")));
	shiroko.push_back(Entity(Vector2f(0, 18), window.loadTexture("res/textures/shiroko/2.png")));
	shiroko.push_back(Entity(Vector2f(0, 18), window.loadTexture("res/textures/shiroko/3.png")));

	innerTexture = window.loadTexture("res/textures/drum/taiko-drum-inner.png");
	outerTexture = window.loadTexture("res/textures/drum/taiko-drum-outer.png");
	outerTextureLeft = window.loadTexture("res/textures/drum/taiko-drum-outer-left.png");
	innerTextureLeft = window.loadTexture("res/textures/drum/taiko-drum-inner-left.png");
	innerTextureRight = window.loadTexture("res/textures/drum/taiko-drum-inner-right.png");
	outerTextureRight = window.loadTexture("res/textures/drum/taiko-drum-outer-right.png");
	glowTexture = window.loadTexture("res/textures/slider/taiko-glow.png");
	perfectTexture =  window.loadTexture("res/textures/taiko-hit-perfect.png");
	okTexture =  window.loadTexture("res/textures/taiko-hit-ok.png");
	missTexture =  window.loadTexture("res/textures/taiko-hit-miss.png");

	font64 = TTF_OpenFont("res/fonts/taikofont.ttf", 64);
	font64_outline = TTF_OpenFont("res/fonts/taikofont.ttf", 64);
	font32 = TTF_OpenFont("res/fonts/taikofont.ttf", 32);
	font32_outline = TTF_OpenFont("res/fonts/taikofont.ttf", 32);
	font24 = TTF_OpenFont("res/fonts/taikofont.ttf", 24);
	font16 = TTF_OpenFont("res/fonts/taikofont.ttf", 16);
	TTF_SetFontOutline(font32_outline, 1);
	TTF_SetFontOutline(font64_outline, 1);  

	chart = getNormalChart(SPAWN_POINT, donTexture, kaTexture);
	timings = getNormalTiming();
	Mix_PlayMusic(reAoharuMusic, -1);
	finalNote = chart.size();
	initNote = SDL_GetTicks();
}

void Gameplay::pressNote(SDL_Texture* p_tex, Uint32 time)
{
	if (playnotes.size() <= 0) return;
	float distance = playnotes[0].distanceFromPoint(playnotes[0].getPos());
	if (playnotes[0].getTex() != p_tex) return;
	if (distance <= 100) 
	{
		if (distance <= 40)
		{
			effect.push_back(Effect(Vector2f(190, PLACEMENT), perfectTexture, time));
			point += PERFECT;
			combo++;
			accs[0]++;
		} else if (distance <= 70)
			{
				effect.push_back(Effect(Vector2f(190, PLACEMENT), okTexture, time));
				point += OKAY;
				accs[1]++;
				combo++;
			} else
				{
					Mix_PlayChannel(-1, missSfx, 0);
					effect.push_back(Effect(Vector2f(190, PLACEMENT), missTexture, time));
					accs[2]++;
					combo = 0;
				}
		playnotes.erase(playnotes.begin());
	}
}

void Gameplay::update()
{		

		currentTime = SDL_GetTicks();
		backGroundTime = currentTime - lastBackGroundTime;
		shirokoTime = currentTime - lastShirokoTime;
		nextNoteTime = timings[currentNote] - GAME_WIDTH + initNote;

		//std::cout << currentTime << " " << nextNoteTime << std::endl;
		//std::cout << currentNote << std::endl;
		if (currentTime >= nextNoteTime)
		{			
			if (currentNote < finalNote) 
			{
				playnotes.push_back(chart[currentNote]);
				currentNote++;
			}
		}

  		Uint32 deltaTime = currentTime - lastFrameTime; // calculate elapsed time
        lastFrameTime = currentTime; // update last frame time

		for (auto it = playnotes.begin(); it != playnotes.end(); ) 
		{
			it->setPos(it->moveNote(it->getPos(), deltaTime)); // set position of the note -> moving
			if (it->getPos().x <= 100 && playnotes.size() > 0) { // if out of screen -> delete and add a miss
				it = playnotes.erase(it);
				effect.push_back(Effect(Vector2f(190, PLACEMENT), missTexture, currentTime));
				Mix_PlayChannel(-1, missSfx, 0);
				accs[2]++;
				combo = 0;
			} else {
				++it;
			}
		}
}

void Gameplay::handleEvents(SDL_Event event)
{
	switch (event.type) 
	{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				case SDLK_LEFT:
					Mix_PlayChannel(-1, kaSfx, 0);
					drum.push_back(Effect(Vector2f(0, PLACEMENT), outerTexture, currentTime));
					pressNote(kaTexture, currentTime);
					break;
				case SDLK_RIGHT:
					Mix_PlayChannel(-1, donSfx, 0);
					drum.push_back(Effect(Vector2f(0, PLACEMENT), innerTexture, currentTime));
					pressNote(donTexture, currentTime);
					break;
				case SDLK_d:
					Mix_PlayChannel(-1, kaSfx, 0);
					drum.push_back(Effect(Vector2f(-2, PLACEMENT), outerTextureLeft, currentTime));
					pressNote(kaTexture, currentTime);
					break;;
				case SDLK_j:
					Mix_PlayChannel(-1, donSfx, 0);
					drum.push_back(Effect(Vector2f(-2, PLACEMENT), innerTextureLeft, currentTime));
					pressNote(donTexture, currentTime);
					break;
				case SDLK_k:
					Mix_PlayChannel(-1, donSfx, 0);
					drum.push_back(Effect(Vector2f(-2, PLACEMENT), innerTextureRight, currentTime));
					pressNote(donTexture, currentTime);
					break;
				case SDLK_f:	
					Mix_PlayChannel(-1, kaSfx, 0);
					drum.push_back(Effect(Vector2f(-2, PLACEMENT), outerTextureRight, currentTime));
					pressNote(kaTexture, currentTime);
					break;
		}
	}
}

void Gameplay::render()
{
	window.clear();
	window.render(backgrounds[0]);
	if (backGroundTime >= 200 && backgrounds.size() > 0) 
	{
		backgrounds.erase(backgrounds.begin());
		lastBackGroundTime = currentTime;
	} // background timer

	for (Entity& p : slider) window.render(p);
	if (currentTime - initNote > 95245 && currentTime - initNote < 116578) window.render(Vector2f(190,PLACEMENT), glowTexture);

	window.render(Vector2f(1140, 20), getScore(point), font32, blue);
	window.render(Vector2f(1175, 60), getScore(accs[0], accs[1], accs[2]), font24, white);
	window.render(Vector2f(20, 600), getScore("Perfect: ", accs[0]), font24, white);
	window.render(Vector2f(20, 630), getScore("Okay: ", accs[1]), font24, white);
	window.render(Vector2f(20, 660), getScore("Miss: ", accs[2]), font24, white);
	
	window.renderCenter(Vector2f(0, -GAME_HEIGHT/3), getScore("", combo), font64, blue);
	window.renderCenter(Vector2f(0, -GAME_HEIGHT/3+40), "Combo", font24, white);
	window.renderCenter(Vector2f(0, -GAME_HEIGHT/3), getScore("", combo), font64_outline, black);

	for (Effect& p : effect) window.render(p);
	for (Note& p : playnotes) window.render(p);
	window.render(slider[0]);

	if (shirokoTime >= 60) 
	{
		lastShirokoTime = currentTime;
		shirokoBackGround++;
		if (shirokoBackGround == 3) shirokoBackGround = 0;
	} // shiroko timer
	window.render(shiroko[shirokoBackGround]);
	
	for (Effect& p: drum) window.render(p);

	for (Effect& p : effect) {
		if (currentTime - p.getTime() > 100 && effect.size() > 0) effect.erase(effect.begin());
	}
	for (Effect& p : drum) {
		if (currentTime - p.getTime() > 100 && drum.size() > 0) drum.erase(drum.begin());
	}
	
	window.display();
}