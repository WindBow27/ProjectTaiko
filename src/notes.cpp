#include <SDL2/SDL_image.h>
#include <SDL2/SDL_image.h>

#include "Math.hpp"
#include "Entity.hpp"
#include "Notes.hpp"


Notes::Notes(Vector2f(p_pos), SDL_Texture* p_tex)
	: Entity{p_pos,p_tex}
{

}

Vector2f Notes::moveNote(Vector2f p_pos) 
{
	p_pos.x -= velocity;
	//std::cout << p_pos.x << " " << p_pos.y << std::endl;
	return p_pos;
}

bool Notes::isInTheScreen(Vector2f p_pos)
{
	if (p_pos.x <= 0)
	{
		return false;
	}
	return true;
}