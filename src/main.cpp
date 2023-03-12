#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "RenderWindow.hpp"
#include "Entity.hpp"

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) > 0) 
		std::cout << "SDL_Init failed. Error: " << SDL_GetError() << std::endl;

	if (!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_Init failed. Error: " << SDL_GetError() << std::endl;

	RenderWindow window("Project Taiko", 1024, 576);

	SDL_Texture* sliderTexture = window.loadTexture("res/textures/taiko-bar-right.png");
	SDL_Texture* donTexture = window.loadTexture("res/textures/don.png");
	SDL_Texture* kaTexture = window.loadTexture("res/textures/ka.png");
	SDL_Texture* taikoDrumInnerTexture = window.loadTexture("res/textures/taiko-drum-inner");
	SDL_Texture* taikoDrumOuterTexture = window.loadTexture("res/textures/taiko-drum-outer.png");
	SDL_Texture* taikoBarLeftTexture = window.loadTexture("res/textures/taiko-bar-left.png");

	std::vector<Entity> entities = {};
	entities.push_back(Entity(Vector2f(0,100), sliderTexture));
	entities.push_back(Entity(Vector2f(100,200), donTexture));
	entities.push_back(Entity(Vector2f(200,200), kaTexture));
	entities.push_back(Entity(Vector2f(200,200), taikoDrumInnerTexture));
	entities.push_back(Entity(Vector2f(200,200), taikoDrumOuterTexture));
	entities.push_back(Entity(Vector2f(200,200), taikoBarLeftTexture));

	bool gameRunning = true;

	SDL_Event event;

	while (gameRunning)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) 
			{
				if (event.type == SDL_QUIT)
				{
					gameRunning = false;
				}
			}
		}

		window.clear();

		for (Entity& p : entities)
		{
			window.render(p);
		}

		window.display();
	}

	window.cleanUp();
	SDL_Quit();
	
	return 0;
}