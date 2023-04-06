#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Entity.hpp"
#include "RenderWindow.hpp"

class Button:public Entity
{
public:
	Button(RenderWindow& p_window, const char* p_text, 
		TTF_Font* p_font, SDL_Color p_color, SDL_Texture* p_texture, Vector2f p_position);
	SDL_Texture* getTex()
	{ 
		return m_texture; 
	}
	bool getHovered() {return m_hovered; }
	void setSize(Vector2f p_size);
	void setTex(SDL_Texture* p_texture) { m_texture = p_texture; }
	void update();
	void render();
private:
	RenderWindow& m_window;
	const char* m_text;
	SDL_Texture* m_texture;
	TTF_Font* m_font;
	SDL_Color m_color;
	bool m_hovered;
	Vector2f m_size;
	Mix_Chunk* hoveredSfx;
	SDL_Color white = { 255, 255, 255 };
	SDL_Color black = { 0, 0 , 0};
};
