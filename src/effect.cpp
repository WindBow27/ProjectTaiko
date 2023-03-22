#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.hpp"
#include "Entity.hpp"
#include "Effect.hpp"


Effect::Effect(Vector2f(p_pos), SDL_Texture* p_tex, Uint32 p_time)
	: Entity{p_pos,p_tex}, time(p_time)
{

}

Uint32 Effect::getTime() {
	return time;
}
