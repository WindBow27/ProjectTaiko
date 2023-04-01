#include "Game.hpp"
#include "RenderWindow.hpp"

#include <iostream>

TitleScreen* title = NULL;
Gameplay* gameplay = NULL;

RenderWindow window;

Game::Game()
{

}

Game::~Game()
{

}

enum State
{
	Title = 0,
	Ingame = 1,
	End = 2,
	Option = 3
};

void Game::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) > 0) 
		std::cout << "SDL_Init failed. Error: " << SDL_GetError() << std::endl;
	if (!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_Init failed. Error: " << SDL_GetError() << std::endl;
	if (Mix_Init(MIX_INIT_MP3) < 0)
		std::cout << "Mix_Init failed. Error: " << SDL_GetError() << std::endl;
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		std::cout << "Mix_OpenAudio failed. Error: " << SDL_GetError() << std::endl;
	if (TTF_Init())
		std::cout << "TTF_init has failed. Error: " << SDL_GetError() << std::endl;
	
	window.create("Project Taiko", 1280, 720);

	gameState = Title;
	title = new TitleScreen(window);
	gameplay = new Gameplay(window);

	isRunning = true;
}

void Game::update() {
	switch (gameState)
	{
	case Title:
		title->update();
		break;
	case Ingame:
		gameplay->update();
		break;
	default:
		break;
	}
}

void Game::handleEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch(e.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (gameState == Title)
			{
				gameState = Ingame;
				gameplay->init();
			}
			if (gameState == Ingame)
			{
				//gameplay->handleEvents(e);
			}
		case SDL_KEYDOWN:
			if (gameState == Ingame)
			{
				gameplay->handleEvents(e);
			}
		}
	}
}

void Game::clean()
{
	window.cleanUp();
	SDL_Quit();
}

void Game::render()
{
	switch(gameState)
	{
	case Title:
		title->render();
		break;
	case Ingame:
		gameplay->render();
		break;
	default:
		break;
	}
}

bool Game::running() {
	return isRunning;
}