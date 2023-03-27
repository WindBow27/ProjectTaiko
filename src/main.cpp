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

const int GAME_WIDTH = 1280;
const int GAME_HEIGHT = 720;
const int PERFECT = 300;
const int OKAY = 100;
const int PLACEMENT = GAME_HEIGHT/4+5;
int point = 0;
int combo = 0;
Vector2f SPAWN_POINT = {290 + GAME_WIDTH, 235};

RenderWindow window;

std::vector<Entity> slider;
std::vector<Entity> backgrounds;
std::vector<Entity> shiroko;
std::vector<Effect> effect;
std::vector<Effect> drum;
std::vector<Note> chart;
std::vector<Note> playnotes;
std::vector<Uint32> timings;
std::vector<int> accs(3);
std::vector<const char*> bgfile;

Mix_Chunk* donSfx;
Mix_Chunk* kaSfx ;
Mix_Chunk* missSfx;
Mix_Music* reAoharuMusic;

SDL_Texture* donTexture;
SDL_Texture* kaTexture;
SDL_Texture* innerTexture;
SDL_Texture* outerTexture;
SDL_Texture* outerTextureLeft;
SDL_Texture* innerTextureLeft;
SDL_Texture* innerTextureRight;
SDL_Texture* outerTextureRight;
SDL_Texture* glowTexture;
SDL_Texture* perfectTexture;
SDL_Texture* okTexture;
SDL_Texture* missTexture;

TTF_Font* font64;
TTF_Font* font64_outline;
TTF_Font* font32;
TTF_Font* font32_outline;
TTF_Font* font24;
TTF_Font* font16;

SDL_Color white = { 255, 255, 255 };
SDL_Color black = { 0, 0, 0 };
SDL_Color silver = { 178, 192, 205 };
SDL_Color blue = {1, 215, 251 };

void init()
{
	if (SDL_Init(SDL_INIT_VIDEO) > 0) 
		std::cout << "SDL_Init failed. Error: " << SDL_GetError() << std::endl;
	if (!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_Init failed. Error: " << SDL_GetError() << std::endl;
	if (Mix_Init(MIX_INIT_MP3) < 0)
		std::cout << "Mix_Init failed. Error: " << SDL_GetError() << std::endl;
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		std::cout << "Mix_OpenAudio failed. Error: " << SDL_GetError() << std::endl;
	TTF_Init();

	window.create("Project Taiko", GAME_WIDTH, GAME_HEIGHT);

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
}

void pressNote(SDL_Texture* p_tex, Uint32 time)
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

void gameLoop()
{
	bool gameRunning = true;
	int currentNote = 0;
	int finalNote = chart.size();
	int shirokoBackGround = 0;

	Uint32 currentTime;

	Uint32 backGroundTime;
	Uint32 shirokoTime;
	Uint32 lastBackGroundTime = 0;
	Uint32 lastShirokoTime = 0;

	Uint32 initNote = SDL_GetTicks();
	Uint32 nextNoteTime;
	Uint32 lastFrameTime = SDL_GetTicks();

	Mix_PlayMusic(reAoharuMusic, -1);

	SDL_Event event;

	while (gameRunning)
	{
		window.clear();

		currentTime = SDL_GetTicks();
		backGroundTime = currentTime - lastBackGroundTime;
		shirokoTime = currentTime - lastShirokoTime;
		nextNoteTime = timings[currentNote] - GAME_WIDTH + initNote;

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
		
		while (SDL_PollEvent(&event))
		{
			switch (event.type) 
			{
				case SDL_QUIT:
					gameRunning = false;
					break;
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
						case SDLK_z:
							Mix_PlayChannel(-1, kaSfx, 0);
							drum.push_back(Effect(Vector2f(-2, PLACEMENT), outerTextureLeft, currentTime));
							pressNote(kaTexture, currentTime);
							break;;
						case SDLK_x:
							Mix_PlayChannel(-1, donSfx, 0);
							drum.push_back(Effect(Vector2f(-2, PLACEMENT), innerTextureLeft, currentTime));
							pressNote(donTexture, currentTime);
							break;
						case SDLK_c:
							Mix_PlayChannel(-1, donSfx, 0);
							drum.push_back(Effect(Vector2f(-2, PLACEMENT), innerTextureRight, currentTime));
							pressNote(donTexture, currentTime);
							break;
						case SDLK_v:	
							Mix_PlayChannel(-1, kaSfx, 0);
							drum.push_back(Effect(Vector2f(-2, PLACEMENT), outerTextureRight, currentTime));
							pressNote(kaTexture, currentTime);
							break;
				}
			}
		}

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
}


int main(int argc, char* argv[])
{
	init();
	gameLoop();
	window.cleanUp();
	Mix_CloseAudio();
	TTF_CloseFont(font64);
	TTF_CloseFont(font64_outline);
	TTF_CloseFont(font32);
	TTF_CloseFont(font32_outline);
	TTF_CloseFont(font24);
	TTF_CloseFont(font16);
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();

	return 0;
}