#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "RenderWindow.hpp"

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) > 0) 
		std::cout << "SDL_Init failed. Error: " << SDL_GetError() << std::endl;

	if (!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_Init failed. Error: " << SDL_GetError() << std::endl;

	RenderWindow window("Project Taiko", 1280, 720);

	return 0;
}