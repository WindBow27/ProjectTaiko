#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "RenderWindow.hpp"
#include "Entity.hpp"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

RenderWindow::RenderWindow()
{

}

void RenderWindow::create(const char* p_title, int p_w, int p_h)
{
    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);

    if (window == NULL)
        std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;

    // Set render hint to enable VSync
    if (SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1") == SDL_FALSE) {
        std::cout << "Failed to enable VSync. Error: " << SDL_GetError() << std::endl;
    }

    // Set render scale quality to nearest
    if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0") == SDL_FALSE) {
        std::cout << "Failed to set render scale quality to nearest. Error: " << SDL_GetError() << std::endl;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}


SDL_Texture* RenderWindow::loadTexture(const char* p_filePath)
{
	SDL_Texture* texture = NULL; 
	texture = IMG_LoadTexture(renderer, p_filePath);

	if (texture == NULL)
		std::cout << "Failed to load texture. Error: " << SDL_GetError() << std::endl;

	return texture;
}

void RenderWindow::clear()
{
	SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
	SDL_RenderClear(renderer);
}

void RenderWindow::render(Entity& p_entity)
{
	SDL_Rect src;
	src.x = p_entity.getCurrentFrame().x;
	src.y = p_entity.getCurrentFrame().y;
	src.w = p_entity.getCurrentFrame().w;
	src.h = p_entity.getCurrentFrame().h;

	SDL_Rect dst;
	dst.x = p_entity.getPos().x;
	dst.y = p_entity.getPos().y;
	dst.w = p_entity.getCurrentFrame().w;
	dst.h = p_entity.getCurrentFrame().h;

	SDL_RenderCopy(renderer, p_entity.getTex(), &src, &dst);
}

void RenderWindow::render(Vector2f(p_pos), SDL_Texture* p_tex)
{
	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	SDL_QueryTexture(p_tex, NULL, NULL, &src.w, &src.h); 

	SDL_Rect dst;
	dst.x = p_pos.getX();
	dst.y = p_pos.getY();
	dst.w = src.w;
	dst.h = src.h;

	SDL_RenderCopy(renderer, p_tex, &src, &dst);
}

void RenderWindow::render(SDL_Texture* p_tex)
{
	render(Vector2f(0,0), p_tex);
}

void RenderWindow::render(Vector2f(p_pos), const char* p_text, TTF_Font* font, SDL_Color textColor)
{
		SDL_Surface* surfaceMessage = TTF_RenderText_Blended( font, p_text, textColor);
		SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

		SDL_Rect src;
		src.x = 0;
		src.y = 0;
		src.w = surfaceMessage->w;
		src.h = surfaceMessage->h; 

		SDL_Rect dst;
		dst.x = p_pos.getX();
		dst.y = p_pos.getY();
		dst.w = src.w;
		dst.h = src.h;

		SDL_RenderCopy(renderer, message, &src, &dst);
		SDL_DestroyTexture(message);
		SDL_FreeSurface(surfaceMessage);
}

void RenderWindow::renderCenter(Vector2f(p_pos), const char* p_text, TTF_Font* font, SDL_Color textColor)
{
		SDL_Surface* surfaceMessage = TTF_RenderText_Blended( font, p_text, textColor);
		SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

		SDL_Rect src;
		src.x = 0;
		src.y = 0;
		src.w = surfaceMessage->w;
		src.h = surfaceMessage->h; 

		SDL_Rect dst;
		dst.x = SCREEN_WIDTH/2 - src.w/2 + p_pos.getX();
		dst.y = SCREEN_HEIGHT/2 - src.h/2 + p_pos.getY();
		dst.w = src.w;
		dst.h = src.h;

		SDL_RenderCopy(renderer, message, &src, &dst);
		SDL_DestroyTexture(message);
		SDL_FreeSurface(surfaceMessage);
}

void RenderWindow::display()
{
	SDL_RenderPresent(renderer);
}

void RenderWindow::cleanUp()
{
	SDL_DestroyWindow(window);
}