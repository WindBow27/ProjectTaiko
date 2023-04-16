#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <fstream>
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
    donSfx = Mix_LoadWAV("res/sounds/taiko-normal-hitnormal.wav");
	kaSfx = Mix_LoadWAV("res/sounds/taiko-normal-hitclap.wav");
	bigSfx = Mix_LoadWAV("res/sounds/taiko-normal-hitfinish.wav");
	missSfx = Mix_LoadWAV("res/sounds/taiko-normal-hitwhistle.wav");
	reAoharuMusic = Mix_LoadMUS("res/sounds/reaoharu-audio.mp3");

	donTexture = window.loadTexture("res/textures/gameplay/don.png");
	kaTexture = window.loadTexture("res/textures/gameplay/ka.png");
	bigDonTexture = window.loadTexture("res/textures/gameplay/big-don.png");
	bigKaTexture = window.loadTexture("res/textures/gameplay/big-ka.png");

	slider.push_back(Entity(Vector2f(0, PLACEMENT), window.loadTexture("res/textures/slider/taiko-bar-left.png")));
	slider.push_back(Entity(Vector2f(190, PLACEMENT), window.loadTexture("res/textures/slider/taiko-bar-middle.png")));
	slider.push_back(Entity(Vector2f(390, PLACEMENT), window.loadTexture("res/textures/slider/taiko-bar-right.png")));
	
	//Background textures
	/*for (int i = 1; i <= 723; i++) {
        std::vector<const char*> v = getText("res/textures/background/", i, ".png");
        bgfile.insert(bgfile.end(), v.begin(), v.end());
    }
    for (auto i: bgfile) backgrounds.push_back(Entity(Vector2f(0,0), window.loadTexture(i)));
    for (auto i : bgfile) delete[] i; // Free memory for const char**/

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
	perfectTexture =  window.loadTexture("res/textures/gameplay/taiko-hit-perfect.png");
	okTexture =  window.loadTexture("res/textures/gameplay/taiko-hit-ok.png");
	missTexture =  window.loadTexture("res/textures/gameplay/taiko-hit-miss.png");

	binding001 = window.loadTexture("res/textures/gameplay/001.png");
	binding002 = window.loadTexture("res/textures/gameplay/002.png");
	binding003 = window.loadTexture("res/textures/gameplay/003.png");
	binding004 = window.loadTexture("res/textures/gameplay/004.png");
	arisBlindingLight = window.loadTexture("res/textures/gameplay/blinding-light.png");

	font64 = TTF_OpenFont("res/fonts/taikofont.ttf", 64);
	font64_outline = TTF_OpenFont("res/fonts/taikofont.ttf", 64);
	font32 = TTF_OpenFont("res/fonts/taikofont.ttf", 32);
	font32_outline = TTF_OpenFont("res/fonts/taikofont.ttf", 32);
	font24 = TTF_OpenFont("res/fonts/taikofont.ttf", 24);
	font16 = TTF_OpenFont("res/fonts/taikofont.ttf", 16);
	TTF_SetFontOutline(font32_outline, 1);
	TTF_SetFontOutline(font64_outline, 1);  
}

void Gameplay::init(int difficulty, std::vector<bool>mod)
{
	chart.clear();
	timings.clear();
	playnotes.clear();
	chart = getChart(SPAWN_POINT, SPAWN_POINT_BIG, donTexture, kaTexture, bigDonTexture, bigKaTexture, difficulty);
	timings = getTiming(difficulty);
	accs = { 0, 0, 0};
	point = 0;
	combo = 0;
	maxCombo = 0;
	bindingCount = 0;
	Mix_PlayMusic(reAoharuMusic, -1);
	finalNote = chart.size();
	initNote = SDL_GetTicks();
	currentNote = 0;
	shirokoBackGround = 0;
	backGroundCount = 1;
	lastBackGroundTime = 0;
	lastShirokoTime = 0;
	lastFrameTime = SDL_GetTicks();

	for (auto i:mod) if (i) bindingCount++;
	precise = (mod[0])?2:1;
	suddenDeath = (mod[1])?true:false;
	allPerfect = (mod[2])?true:false;
	bindingLight = (mod[3])?true:false;
	//std::cout << precise << suddenDeath << allPerfect << bindingLight << std::endl;

	std::fstream scoreFile("data/offset.txt");
	scoreFile >> offset;
	scoreFile.close();
	if (offset < -50) offset = -50;
	else if (offset > 50) offset = 50;
	//std::cout << offset << std::endl;
}

void Gameplay::pressNote(SDL_Texture* p_tex, SDL_Texture* big_p_tex, Uint32 time)
{
	if (playnotes.size() <= 0) return;
	float distance = playnotes[0].distanceFromPoint(playnotes[0].getPos(), offset);
	//std::cout << distance << std::endl;
	if (!((playnotes[0].getTex() == p_tex) || (playnotes[0].getTex() == big_p_tex))) return;
	if (distance <= 100/precise) 
	{
		if (playnotes[0].getTex() == bigDonTexture || playnotes[0].getTex() == bigKaTexture)
			Mix_PlayChannel(-1, bigSfx, 0);

		if (distance <= 30/precise)
		{
			effect.push_back(Effect(Vector2f(190, PLACEMENT), perfectTexture, time));
			point += POINT_PERFECT;
			combo++;
			accs[PERFECT]++;
		} else if (distance <= 60/precise)
			{
				effect.push_back(Effect(Vector2f(190, PLACEMENT), okTexture, time));
				point += POINT_OKAY;
				accs[OKAY]++;
				combo++;
			} else
				{
					Mix_PlayChannel(-1, missSfx, 0);
					effect.push_back(Effect(Vector2f(190, PLACEMENT), missTexture, time));
					accs[MISS]++;
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
				accs[MISS]++;
				maxCombo = std::max(maxCombo, combo);
				combo = 0;
			} else {
				++it;
			}
		}


	std::string v = getTextString("res/textures/background/", backGroundCount, ".png");
	SDL_DestroyTexture(backGroundTexture); //insane memory leaks
	backGroundTexture = window.loadTexture(v.c_str());
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
					pressNote(kaTexture, bigKaTexture, currentTime);
					break;
				case SDLK_RIGHT:
					Mix_PlayChannel(-1, donSfx, 0);
					drum.push_back(Effect(Vector2f(0, PLACEMENT), innerTexture, currentTime));
					pressNote(donTexture, bigDonTexture, currentTime);
					break;
				case SDLK_d:
					Mix_PlayChannel(-1, kaSfx, 0);
					drum.push_back(Effect(Vector2f(-2, PLACEMENT), outerTextureLeft, currentTime));
					pressNote(kaTexture, bigKaTexture, currentTime);
					break;;
				case SDLK_j:
					Mix_PlayChannel(-1, donSfx, 0);
					drum.push_back(Effect(Vector2f(-2, PLACEMENT), innerTextureLeft, currentTime));
					pressNote(donTexture, bigDonTexture, currentTime);
					break;
				case SDLK_k:
					Mix_PlayChannel(-1, donSfx, 0);
					drum.push_back(Effect(Vector2f(-2, PLACEMENT), innerTextureRight, currentTime));
					pressNote(donTexture, bigDonTexture, currentTime);
					break;
				case SDLK_f:	
					Mix_PlayChannel(-1, kaSfx, 0);
					drum.push_back(Effect(Vector2f(-2, PLACEMENT), outerTextureRight, currentTime));
					pressNote(kaTexture, bigKaTexture, currentTime);
					break;
		}
	}
}

void Gameplay::render()
{
	window.clear();
	window.render(Vector2f(0,0), backGroundTexture);
	if (backGroundTime >= 200 && backGroundCount < 723) 
	{
		lastBackGroundTime = currentTime;
		backGroundCount++;
	} // background timer

	for (Entity& p : slider) window.render(p);
	if (currentTime - initNote > 95245 && currentTime - initNote < 116578) window.render(Vector2f(190,PLACEMENT), glowTexture);

	window.render(Vector2f(1140, 20), getScore(point), font32, blue);
	window.render(Vector2f(1175, 60), getScore(accs[PERFECT], accs[OKAY], accs[MISS]), font24, white);
	//window.render(Vector2f(20, 600), getScore("Perfect: ", accs[PERFECT]), font24, white);
	//window.render(Vector2f(20, 630), getScore("Okay: ", accs[OKAY]), font24, white);
	//window.render(Vector2f(20, 660), getScore("Miss: ", accs[MISS]), font24, white);
	
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
	
	//bindings
	if (precise == 2) window.render(Vector2f(1160, 100), binding001);
	if (suddenDeath) window.render(Vector2f(1190, 100), binding002);
	if (allPerfect) window.render(Vector2f(1220, 100), binding003);
	if (bindingLight) {
		window.render(Vector2f(1250, 100), binding004);
		window.render(Vector2f(0,0), arisBlindingLight);
	}

	for (Effect& p: drum) window.render(p);

	for (Effect& p : effect) {
		if (currentTime - p.getTime() > 100 && effect.size() > 0) effect.erase(effect.begin());
	}
	for (Effect& p : drum) {
		if (currentTime - p.getTime() > 100 && drum.size() > 0) drum.erase(drum.begin());
	}
	
	window.display();
}

std::vector<int> Gameplay::getAcc()
{
	return accs;
}

int Gameplay::getEnd()
{
	return backGroundCount;
}

int Gameplay::getTotalScore()
{
	return point;
}

int Gameplay::getMaxCombo()
{
	return maxCombo;
}

int Gameplay::getRanking()
{
	int rank = getRank(accs[PERFECT], accs[OKAY], accs[MISS]);
	return rank;
}

bool Gameplay::getMiss()
{
	if (accs[MISS] > 0 && suddenDeath) return true;
	return false;
}

bool Gameplay::getOkay()
{
	if (accs[OKAY] > 0 && allPerfect) return true;
	return false;
}

int Gameplay::getBindingCount()
{
	return bindingCount;
}