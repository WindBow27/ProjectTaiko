#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Entity.hpp"
#include "Math.hpp"
#include "Note.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <cstring>

std::vector<Note> getChart(Vector2f SPAWN_POINT, Vector2f SPAWN_POINT_BIG, SDL_Texture* donTexture, SDL_Texture* kaTexture, SDL_Texture* bigDonTexture, SDL_Texture* bigKaTexture, int difficulty);
std::vector<Uint32> getTiming(int difficulty);
