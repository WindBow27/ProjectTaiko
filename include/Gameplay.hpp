#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Math.hpp"
#include "Note.hpp"
#include "Effect.hpp"
#include "Utils.hpp"
#include "Charts.hpp"

enum Accuracy
{
	PERFECT = 0,
	OKAY = 1, 
	MISS = 2,
};

class Gameplay
{
public:
	Gameplay(RenderWindow p_window);
	void init(int difficulty, std::vector<bool> mod);
	void pressNote(SDL_Texture* p_tex, SDL_Texture* big_p_tex, Uint32 time);
	void update();
	void handleEvents(SDL_Event event);
	void render();
	void reset();
	int getTotalScore();
	int getEnd();
	std::vector<int> getAcc();
	int getMaxCombo();
	int getRanking();
	bool getMiss();
	bool getOkay();
	int getBindingCount();
private:
	RenderWindow window;
	const int GAME_WIDTH = 1280;
	const int GAME_HEIGHT = 720;
	const int POINT_PERFECT = 300;
	const int POINT_OKAY = 100;
	const int PLACEMENT = GAME_HEIGHT/4+5;
	int point;
	int combo;
	int maxCombo;
	Vector2f SPAWN_POINT = {1570, 235};
	Vector2f SPAWN_POINT_BIG = {1570, 221};

	std::vector<Entity> slider;
	std::vector<Entity> backgrounds;
	std::vector<Entity> shiroko;
	std::vector<Effect> effect;
	std::vector<Effect> drum;
	std::vector<Note> chart;
	std::vector<Note> playnotes;
	std::vector<Uint32> timings;
	std::vector<int> accs;
	//std::vector<const char*> bgfile;

	Mix_Chunk* donSfx;
	Mix_Chunk* kaSfx ;
	Mix_Chunk* bigSfx;
	Mix_Chunk* missSfx;
	Mix_Music* reAoharuMusic;

	SDL_Texture* donTexture;
	SDL_Texture* kaTexture;
	SDL_Texture* bigDonTexture;
	SDL_Texture* bigKaTexture;
	SDL_Texture* innerTexture;
	SDL_Texture* outerTexture;
	SDL_Texture* outerTextureLeft;
	SDL_Texture* innerTextureLeft;
	SDL_Texture* innerTextureRight;
	SDL_Texture* outerTextureRight;
	SDL_Texture* glowTexture;
	SDL_Texture* perfectTexture;
	SDL_Texture* okTexture;
	SDL_Texture* missTexture;
	SDL_Texture* backGroundTexture;
	SDL_Texture* binding001;
	SDL_Texture* binding002;
	SDL_Texture* binding003;
	SDL_Texture* binding004;
	SDL_Texture* arisBlindingLight;

	TTF_Font* font64;
	TTF_Font* font64_outline;
	TTF_Font* font32;
	TTF_Font* font32_outline;
	TTF_Font* font24;
	TTF_Font* font16;

	SDL_Color white = { 255, 255, 255 };
	SDL_Color black = { 0, 0, 0 };
	SDL_Color silver = { 178, 192, 205 };
	SDL_Color blue = {1, 215, 251 };

	int currentNote;
	int shirokoBackGround;
	int backGroundCount;
	int finalNote;
	int bindingCount;
	float offset;

	Uint32 currentTime;

	Uint32 backGroundTime;
	Uint32 shirokoTime;
	Uint32 lastBackGroundTime;
	Uint32 lastShirokoTime;

	Uint32 initNote;
	Uint32 nextNoteTime;
	Uint32 lastFrameTime;

	//Mods
	float precise;
	bool suddenDeath;
	bool allPerfect;
	bool bindingLight;
};