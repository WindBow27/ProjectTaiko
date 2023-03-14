#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Notes.hpp"

const int GAME_HEIGHT = 720;
const int GAME_WIDTH = 1280;

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) > 0) 
		std::cout << "SDL_Init failed. Error: " << SDL_GetError() << std::endl;

	if (!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_Init failed. Error: " << SDL_GetError() << std::endl;

	RenderWindow window("Project Taiko", 1280, 720);

	SDL_Texture* background = window.loadTexture("res/textures/background.png");
	SDL_Texture* taikoBarLeftTexture = window.loadTexture("res/textures/taiko-bar-left.png");
	SDL_Texture* taikoBarMiddleTexture = window.loadTexture("res/textures/taiko-bar-middle.png");
	SDL_Texture* taikoBarRightTexture = window.loadTexture("res/textures/taiko-bar-right.png");
	SDL_Texture* donTexture = window.loadTexture("res/textures/don.png");
	SDL_Texture* kaTexture = window.loadTexture("res/textures/ka.png");
	//SDL_Texture* taikoDrumInnerTexture = window.loadTexture("res/textures/taiko-drum-inner.png");
	//SDL_Texture* taikoDrumOuterTexture = window.loadTexture("res/textures/taiko-drum-outer.png");

	std::vector<Entity> entities = {};
	entities.push_back(Entity(Vector2f(0,0), background));
	entities.push_back(Entity(Vector2f(0, GAME_HEIGHT/4), taikoBarLeftTexture));
	entities.push_back(Entity(Vector2f(190, GAME_HEIGHT/4), taikoBarMiddleTexture));
	entities.push_back(Entity(Vector2f(390, GAME_HEIGHT/4), taikoBarRightTexture));
	//entities.push_back(Entity(Vector2f(500, 300), taikoDrumInnerTexture));
	//entities.push_back(Entity(Vector2f(0,200), taikoDrumOuterTexture));
	
	std::vector<Notes> notes = {};
	notes.push_back(Notes(Vector2f(1200, 217), donTexture));
	notes.push_back(Notes(Vector2f(190, 217), kaTexture));
	

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


   		/*SDL_Point mouse_position; 
		SDL_GetMouseState(                   
        &mouse_position.x,               
        &mouse_position.y
   		);
		SDL_Log("Mouse position: x=%i y=%i",  
        mouse_position.x, mouse_position.y
    	);*/

		window.clear();

		for (Entity& p : entities)
		{
			window.render(p);
		}

		notes[0].getPos() = notes[0].moveNote(notes[0].getPos());
		if (notes[0].isInTheScreen(notes[0].getPos())) 
		{
			window.render(notes[0]);
		}
		window.render(entities[1]);
		window.display();
	}

	window.cleanUp();
	SDL_Quit();
	
	return 0;
}