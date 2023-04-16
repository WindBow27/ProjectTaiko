#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Entity.hpp"

class RenderWindow 
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
public:
	RenderWindow();
	void create(const char* p_title, int p_w, int p_h);
	SDL_Texture* loadTexture(const char* p_filePath);
	void clear();
	void render(Entity& p_entity);
	void render(Vector2f(p_pos), SDL_Texture* p_tex);
	void render(SDL_Texture* p_tex);
	void render(Vector2f(p_pos), const char* p_text, TTF_Font* font, SDL_Color textColor);
	void renderCenter(Vector2f(p_pos), const char* p_text, TTF_Font* font, SDL_Color textColor);
	void render(Vector2f(p_pos), Vector2f(p_size), SDL_Texture* p_tex);
	void render(Vector2f(p_pos), SDL_Rect(p_rect), SDL_Texture* p_tex);
	void display();
	void cleanUp();
};
