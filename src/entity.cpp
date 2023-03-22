#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Entity.hpp"
#include "Math.hpp"

Entity::Entity(Vector2f p_pos, SDL_Texture* p_tex)
//member initializer lists
:pos(p_pos), tex(p_tex)
{
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = 86;
	currentFrame.h = 86;
	SDL_QueryTexture(p_tex, NULL, NULL, &currentFrame.w, &currentFrame.h);
}

Vector2f& Entity::getPos()
{
	return pos;
}

SDL_Texture* Entity::getTex()
{
	return tex;
}

SDL_Rect Entity::getCurrentFrame() 
{
	return currentFrame;
}

void Entity::setPos(Vector2f(p_pos))
{
	pos.x = p_pos.x;
	pos.y = p_pos.y;
}

void Entity::setScale(float w, float h)
{
	scale.x = w;
	scale.y = h;
}

