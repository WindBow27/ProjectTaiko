#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.hpp"
#include "Entity.hpp"

class Note: public Entity
{
public:
	Note(Vector2f(p_pos), SDL_Texture* p_tex);
	Vector2f moveNote(Vector2f p_pos, Uint32 deltaTime);
	bool isInTheScreen(Vector2f p_pos);
	float distanceFromPoint(Vector2f p_pos);
	float getVelocity();
private:
	float velocity = 1000.0f;
};