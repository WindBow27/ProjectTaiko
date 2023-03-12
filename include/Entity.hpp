#pragma once
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_image.h>

#include "Math.hpp"

class Entity 
{
public:
	//constructor
	Entity(Vector2f p_pos, SDL_Texture* p_tex);
	Vector2f& getPos()
	{
		return pos;
	}
	void setPos(float x, float y);
	void setScale(float w, float h);
	SDL_Texture* getTex();
	SDL_Rect getCurrentFrame();
private: 
	Vector2f pos;
	Vector2f scale = Vector2f(1, 1);
	SDL_Rect currentFrame;
	SDL_Texture* tex;
};