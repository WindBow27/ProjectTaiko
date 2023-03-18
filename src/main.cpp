#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <vector>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Note.hpp"

const int GAME_WIDTH = 1280;
const int GAME_HEIGHT = 720;
const Vector2f SPAWN_POINT = {500,230};
const int NOTE_DURATION = 1000;

RenderWindow window;

std::vector<Entity> base;
std::vector<Entity> drumtexture;
std::vector<Note> notes;

Mix_Chunk* donSfx;
Mix_Chunk* kaSfx;

SDL_Texture* donTexture;
SDL_Texture* kaTexture;

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

	window.create("Project Taiko", GAME_WIDTH, GAME_HEIGHT);

	donSfx = Mix_LoadWAV("res/sounds/taiko-normal-hitnormal.wav");
	kaSfx = Mix_LoadWAV("res/sounds/taiko-normal-hitclap.wav");

	donTexture = window.loadTexture("res/textures/don.png");
	kaTexture = window.loadTexture("res/textures/ka.png");

	base.push_back(Entity(Vector2f(0,0), window.loadTexture("res/textures/background.png")));
	base.push_back(Entity(Vector2f(0, GAME_HEIGHT/4), window.loadTexture("res/textures/taiko-bar-left.png")));
	base.push_back(Entity(Vector2f(190, GAME_HEIGHT/4), window.loadTexture("res/textures/taiko-bar-middle.png")));
	base.push_back(Entity(Vector2f(390, GAME_HEIGHT/4), window.loadTexture("res/textures/taiko-bar-right.png")));
	
	drumtexture.push_back(Entity(Vector2f(300, 300), window.loadTexture("res/textures/taiko-drum-inner.png")));
	drumtexture.push_back(Entity(Vector2f(300, 300), window.loadTexture("res/textures/taiko-drum-outer.png")));
}

void reAoharu()
{
	notes.push_back(Note(SPAWN_POINT, donTexture));
	notes.push_back(Note(SPAWN_POINT, kaTexture));
	notes.push_back(Note(SPAWN_POINT, donTexture));
	notes.push_back(Note(SPAWN_POINT, kaTexture)); 
}

void timings()
{

}

void gameLoop()
{
	bool gameRunning = true;
	Uint32 nextNoteTime = SDL_GetTicks() + NOTE_DURATION;

	SDL_Event event;

	while (gameRunning)
	{
		window.clear();


		Uint32 currentTime = SDL_GetTicks();

		if (currentTime >= nextNoteTime)
		{
			

		}

		for (auto it = notes.begin(); it != notes.end(); ) 
		{
			it->setPos(it->moveNote(it->getPos()));
			if (it->getPos().x <= 0) {
				it = notes.erase(it);
			} else {
				++it;
			}
		}
		
		window.render(base[1]);

		while (SDL_PollEvent(&event))
		{
			switch (event.type) {
				case SDL_QUIT:
					gameRunning = false;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_LEFT:
							Mix_PlayChannel(-1, kaSfx, 0);
							// std::cout << notes[0].distanceFromPoint(notes[0].getPos()) << std::endl;
							// if (notes[0].distanceFromPoint(notes[0].getPos()) <= 50 && notes[0].getTex()) 
							// {
							// 	std::cout << notes[0].getTex() << std::endl;
							// 	notes[0].setPos(-100, -100); 
							// }
							break;
						case SDLK_RIGHT:
							Mix_PlayChannel(-1, donSfx, 0);
							break;
					}
			}
		}

		for (Entity& p : base) window.render(p);
		//for (Entity&p : drumtexture) window.render(p);
		for (Entity&p : notes) window.render(p);
   		window.render(base[1]);

		window.display();
	}
}


int main(int argc, char* argv[])
{
	init();
	reAoharu();
	timings();
	gameLoop();
	window.cleanUp();
	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();

	return 0;
}