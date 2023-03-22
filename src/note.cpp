#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.hpp"
#include "Entity.hpp"
#include "Note.hpp"


Note::Note(Vector2f(p_pos), SDL_Texture* p_tex)
	: Entity{p_pos,p_tex}
{

}

Vector2f Note::moveNote(Vector2f p_pos, Uint32 deltaTime) 
{
	p_pos.x -= velocity * deltaTime / 1000.0f;
	//std::cout << p_pos.x << " " << p_pos.y << std::endl;
	return p_pos;
}

bool Note::isInTheScreen(Vector2f p_pos)
{
	if (p_pos.x <= 0)
	{
		return false;
	}
	return true;
}

float Note::distanceFromPoint(Vector2f p_pos)
{	
	//std::cout << p_pos.x - 290;
	return (p_pos.x - 290);
}

float Note::getVelocity()
{
	return velocity;
}