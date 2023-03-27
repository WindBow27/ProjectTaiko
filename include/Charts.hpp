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

std::vector<Note> getEasyChart(Vector2f p_pos, SDL_Texture* donTexture, SDL_Texture* kaTexture);
std::vector<Uint32> getEasyTiming();

std::vector<Note> getNormalChart(Vector2f p_pos, SDL_Texture* donTexture, SDL_Texture* kaTexture);
std::vector<Uint32> getNormalTiming();

std::vector<Note> getHardChart(Vector2f p_pos, SDL_Texture* donTexture, SDL_Texture* kaTexture);
std::vector<Uint32> getHardTiming();