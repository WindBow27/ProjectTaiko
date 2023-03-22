#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <vector>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Math.hpp"
#include "Note.hpp"
#include "Effect.hpp"

const int GAME_WIDTH = 1280;
const int GAME_HEIGHT = 720;
Vector2f SPAWN_POINT = {1570,230};

RenderWindow window;

std::vector<Entity> base;
std::vector<Effect> effect;
std::vector<Note> reaoharu;
std::vector<Note> playnotes;
std::vector<Uint32> timings;

Mix_Chunk* donSfx;
Mix_Chunk* kaSfx;
Mix_Music* reAoharuMusic;

SDL_Texture* donTexture;
SDL_Texture* kaTexture;

SDL_Texture* innerTexture;
SDL_Texture* outerTexture;

void init()
{
	if (SDL_Init(SDL_INIT_VIDEO) > 0) 
		std::cout << "SDL_Init failed. Error: " << SDL_GetError() << std::endl;

	if (!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_Init failed. Error: " << SDL_GetError() << std::endl;

	if (Mix_Init(MIX_INIT_MP3) < 0)
		std::cout << "Mix_Init failed. Error: " << SDL_GetError() << std::endl;

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		std::cout << "Mix_OpenAudio failed. Error: " << SDL_GetError() << std::endl;

	window.create("Project Taiko", GAME_WIDTH, GAME_HEIGHT);

	donSfx = Mix_LoadWAV("res/sounds/taiko-normal-hitnormal.wav");
	kaSfx = Mix_LoadWAV("res/sounds/taiko-normal-hitclap.wav");
	reAoharuMusic = Mix_LoadMUS("res/sounds/reaoharu-audio.mp3");

	donTexture = window.loadTexture("res/textures/don.png");
	kaTexture = window.loadTexture("res/textures/ka.png");

	base.push_back(Entity(Vector2f(0,0), window.loadTexture("res/textures/background.png")));
	base.push_back(Entity(Vector2f(0, GAME_HEIGHT/4), window.loadTexture("res/textures/taiko-bar-left.png")));
	base.push_back(Entity(Vector2f(190, GAME_HEIGHT/4), window.loadTexture("res/textures/taiko-bar-middle.png")));
	base.push_back(Entity(Vector2f(390, GAME_HEIGHT/4), window.loadTexture("res/textures/taiko-bar-right.png")));
	
	innerTexture = window.loadTexture("res/textures/taiko-drum-inner.png");
	outerTexture = window.loadTexture("res/textures/taiko-drum-outer.png");
}

void reAoharu()
{
	Note d = {SPAWN_POINT,donTexture};
	Note k = {SPAWN_POINT, kaTexture};

	reaoharu = 
	{
			d,	k,	d,	k,	d,	k,	k,	d,	k,	d,	k,	d,	k,	k,	d,	k,	k,	d,	k,	d,	k,	k,	d,	k,	d,	k,	d,	k,	d,	k,	k,	d,	d,	k,	d,	d,	k,	
	k,	d,	d,	k,	d,	d,	k,	k,	d,	k,	d,	k,	d,	d,	k,	k,	d,	d,	k,	k,	d,	k,	k,	d,	k,	k,	d,	k,	d,	k,	d,	k,	d,	d,	k,	k,	k,	d,	k,	
	d,	k,	d,	k,	k,	k,	d,	k,	k,	d,	k,	d,	k,	k,	d,	k,	k,	k,	d,	k,	d,	k,	d,	k,	k,	k,	k,	d,	d,	k,	d,	d,	k,	k,	d,	k,	k,	k,	k,	
	d,	k,	d,	k,	d,	k,	k,	k,	k,	d,	d,	k,	k,	d,	k,	d,	d,	k,	k,	k,	k,	d,	d,	k,	k,	d,	d,	k,	d,	d,	d,	k,	d,	d,	d,	k,	d,	k,	d,	
	k,	k,	d,	d,	k,	d,	d,	d,	k,	d,	d,	k,	k,	k,	d,	k,	k,	k,	k,	k,	d,	k,	k,	k,	k,	k,	k,	d,	k,	k,	k,	k,	k,	d,	k,	k,	d,	k,	d,	
	k,	k,	d,	d,	k,	d,	d,	k,	k,	d,	d,	k,	k,	d,	d,	k,	d,	k,	d,	k,	d,	d,	k,	d,	k,	k,	d,	d,	k,	d,	k,	d,	k,	k,	d,	d,	d,	d,	k,	
	k,	d,	k,	d,	k,	d,	k,	d,	k,	d,	d,	k,	k,	d,	d,	k,	k,	d,	k,	d,	k,	d,	d,	k,	k,	k,	k,	d,	d,	d,	k,	d,	d,	k,	d,	d,	k,	k,	d,	
	k,	d,	k,	d,	d,	k,	d,	d,	k,	k,	d,	k,	d,	k,	d,	d,	d,	k,	k,	d,	d,	k,	d,	k,	d,	k,	k,	d,	d,	k,	d,	d,	k,	d,	k,	k,	d,	k,	k,	
	d,	k,	d,	d,	k,	k,	d,	d,	k,	k,	d,	d,	d,	k,	d,	k,	k,	d,	k,	d,	k,	d,	d,	k,	d,	d,	k,	d,	k,	d,	k,	d,	d,	k,	d,	d,	d,	d,	k,	
	d,	d,	d,	k,	k,	d,	k,	k,	d,	d,	k,	d,	d,	d,	d,	k,	d,	k,	k,	d, d
	};

	timings = 
	{
		4578,	5078,	5245,	5578,	5912,	6578,	6912,	7245,	7745,	7912,	8245,	8578,	9245,	9412,	9578,	9912,	10412,	10578,	10912,	
		11245,	11912,	12245,	12578,	13078,	13245,	13578,	13912,	14078,	14412,	14578,	14912,	15245,	15912,	16245,	16578,	16912,	17245,	17578,	17912,	18578,	
		18912,	19245,	19578,	19912,	20078,	20245,	20578,	21245,	21578,	21912,	22245,	22578,	22912,	23245,	23412,	23578,	23745,	23912,	24245,	24412,	24578,	
		25912,	26245,	26578,	26745,	26912,	27245,	27578,	27745,	27912,	28078,	28245,	28578,	28912,	29245,	29412,	29578,	29912,	30245,	30412,	30578,	31245,	
		31578,	31912,	32078,	32245,	32578,	32912,	33078,	33245,	33412,	33578,	33912,	34245,	34578,	34745,	34912,	35245,	35578,	35745,	35912,	36578,	36912,	
		37245,	37412,	37578,	37912,	38245,	38412,	38578,	38745,	38912,	39245,	39578,	39745,	39912,	40245,	40578,	40745,	40912,	41078,	41245,	41912,	42245,	
		42578,	42745,	42912,	43245,	43578,	43745,	43912,	44245,	44578,	44912,	45245,	45578,	45912,	46078,	46245,	46412,	46578,	46745,	46912,	47245,	52578,	
		53078,	53578,	53912,	54412,	54912,	55245,	55745,	56245,	56578,	57078,	57578,	57912,	58412,	58912,	59245,	59745,	60245,	60578,	61078,	61578,	61912,	
		62412,	62912,	63245,	63745,	64245,	64745,	65245,	65578,	65912,	66412,	66912,	67412,	67912,	68078,	68245,	68578,	69078,	69578,	70078,	70578,	70912,	
		71245,	71745,	72245,	72578,	72745,	73078,	73245,	73578,	73912,	74578,	74912,	75245,	75745,	75912,	76245,	76578,	77245,	77578,	77745,	77912,	78412,	
		78578,	78745,	78912,	79245,	79912,	80245,	80412,	80578,	81078,	81245,	81578,	81912,	82578,	82912,	83078,	83245,	83745,	83912,	84078,	84245,	84578,	
		85245,	85412,	85578,	85912,	86245,	86412,	86578,	86912,	87245,	87912,	88245,	88412,	88578,	88912,	89245,	89412,	89578,	89912,	90578,	90912,	91245,	
		91745,	91912,	92245,	92578,	92912,	93245,	93412,	93578,	93745,	93912,	94412,	94912,	95245,	95912,	96245,	96578,	96745,	97078,	97245,	97578,	97912,	
		98245,	98412,	98578,	98912,	99078,	99245,	99412,	99745,	99912,	100245,	100578,	101245,	101578,	101745,	101912,	102078,	102412,	102578,	102912,	103245,	103412,	
		103578,	103912,	104245,	104578,	104745,	105078,	105245,	105412,	105578,	105912,	106578,	106912,	107245,	107412,	107745,	107912,	108245,	108412,	108578,	108912,	109245,	
		109412,	109578,	109912,	110078,	110412,	110578,	110745,	110912,	111245,	111912,	112245,	112578,	112745,	113078,	113245,	113578,	113912,	114078,	114245,	114412,	114578,	
		114912,	115078,	115245,	115578,	115745,	116078,	116245,	116578,	119245,	119412,	119578,	119745,	121912,	122078,	122245,	122412,	124578,	124745,	124912,	125078,	127245,	
		127412,	127578,	127745,	129912,	130078,	130245,	130412,	132578,	132745,	132912,	133078,	135245,	135412,	135578,	135745,	135912,	137745
	};
}


void gameLoop()
{
	bool gameRunning = true;
	int currentNote = 0;
	SDL_Event event;
	Uint32 lastFrameTime = SDL_GetTicks();
	Mix_PlayMusic(reAoharuMusic, -1);

	while (gameRunning)
	{
		window.clear();

		Uint32 currentTime = SDL_GetTicks();
		Uint32 nextNoteTime = timings[currentNote] - 1000;

		if (currentTime >= nextNoteTime)
		{			
			playnotes.push_back(reaoharu[currentNote]);
			//std::cout << currentTime << std::endl;
			currentNote++;
		}

  		Uint32 deltaTime = currentTime - lastFrameTime; // calculate elapsed time
  		//std::cout << deltaTime << std::endl;
        lastFrameTime = currentTime; // update last frame time

		for (auto it = playnotes.begin(); it != playnotes.end(); ) 
		{
			it->setPos(it->moveNote(it->getPos(), deltaTime));
			if (it->getPos().x <= 0) {
				it = playnotes.erase(it);
			} else {
				++it;
			}
		}
		
		while (SDL_PollEvent(&event))
		{
			switch (event.type) 
			{
				case SDL_QUIT:
					gameRunning = false;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_LEFT:
							Mix_PlayChannel(-1, kaSfx, 0);
							effect.push_back(Effect(Vector2f(0, GAME_HEIGHT/4), outerTexture, currentTime));
							if (playnotes.size() > 0) {
								if (playnotes[0].distanceFromPoint(playnotes[0].getPos()) <= 50 && playnotes[0].getTex() == kaTexture) 
								{
									playnotes.erase(playnotes.begin());
								}
							}
							break;
						case SDLK_RIGHT:
							Mix_PlayChannel(-1, donSfx, 0);
							effect.push_back(Effect(Vector2f(0, GAME_HEIGHT/4), innerTexture, currentTime));
							if (playnotes.size() > 0) {
								if (playnotes[0].distanceFromPoint(playnotes[0].getPos()) <= 50 && playnotes[0].getTex() == donTexture) 
								{
									playnotes.erase(playnotes.begin());
								}
							}
							break;
				}
			}
		}

		/*int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		std::cout << "Mouse X: " << mouseX << ", Mouse Y: " << mouseY << std::endl;*/

		for (Entity& p : base) window.render(p);
		for (Entity& p : playnotes) window.render(p);
		window.render(base[1]);
		for (Effect& p : effect) window.render(p);

		for (Effect& p : effect) {
			if (currentTime - p.getTime() > 100 && effect.size() > 0) 
			{
				effect.erase(effect.begin());
			}
		}

		window.display();
	}
}


int main(int argc, char* argv[])
{
	init();
	reAoharu();
	gameLoop();
	window.cleanUp();
	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();

	return 0;
}