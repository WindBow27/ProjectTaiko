#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.hpp"
#include "Entity.hpp"

class Effect: public Entity
{
public:
	Effect(Vector2f(p_pos), SDL_Texture* p_tex, Uint32 p_time);
	Uint32 getTime();
private:
	Uint32 time;
};