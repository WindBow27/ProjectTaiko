#pragma once

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_image.h>

#include "Math.hpp"
#include "Entity.hpp"

class Note: public Entity
{
public:
	Note(Vector2f(p_pos), SDL_Texture* p_tex);
	Note spawnDon(SDL_Texture *p_tex);
	Note spawnKa(SDL_Texture *p_tex);
	Vector2f moveNote(Vector2f p_pos);
	bool isInTheScreen(Vector2f p_pos);
	float distanceFromPoint(Vector2f p_pos);
private:
	float velocity = 0.01f;
};