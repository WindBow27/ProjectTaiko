#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_video.h>

#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Math.hpp"
#include "Note.hpp"
#include "Effect.hpp"

const int GAME_WIDTH = 1280;
const int GAME_HEIGHT = 720;
const int PERFECT = 300;
const int OKAY = 100;
const int PLACEMENT = GAME_HEIGHT/4+5;
int point = 0;
int combo = 0;

Vector2f SPAWN_POINT = {290 + GAME_WIDTH, 235};

RenderWindow window;

std::vector<Entity> base;
std::vector<Entity> backgrounds;
std::vector<Entity> playbackground;
std::vector<Entity> shiroko;
std::vector<Effect> effect;
std::vector<Effect> drum;
std::vector<Note> reaoharu;
std::vector<Note> playnotes;
std::vector<Uint32> timings;
std::vector<int> scores(3);
std::vector<const char*> bgfile;

Mix_Chunk* donSfx;
Mix_Chunk* kaSfx ;
Mix_Chunk* missSfx;
Mix_Music* reAoharuMusic;

SDL_Surface* surface;
SDL_Texture* video;

SDL_Texture* donTexture;
SDL_Texture* kaTexture;

SDL_Texture* innerTexture;
SDL_Texture* outerTexture;
SDL_Texture* hyperTexture;
SDL_Texture* perfectTexture;
SDL_Texture* okTexture;
SDL_Texture* missTexture;

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

const char* getScore(const char* str, int score)
{
	std::string s = std::to_string(score);
	s = str + s;
	return s.c_str();
}

const char* getScore(int score)
{
	std::string s = std::to_string(score);
    size_t n = 6;
    int precision = n - std::min(n, s.size());
    s.insert(0, precision, '0');
    return s.c_str();
}

const char* getScore(int score, int totalscore)
{
	float scr = (float)score/totalscore*100.0;
    std::string s = std::to_string(scr);
    s.erase(s.begin()+5,s.end());
    s += "%";
	return s.c_str();
}

const char* getScore(int n300, int n100, int n0)
{
	float acc = (float)(n300*300+n100*100)/((n300+n100+n0)*300)*100.0;
    std::string s = std::to_string(acc);
    s.erase(s.begin()+5,s.end());
    s += "%";
	return s.c_str();
}

std::vector<const char*> getNumber(const char* str, int number, const char* str1)
{
    std::string s = std::to_string(number);
    size_t n = 3;
    int precision = n - std::min(n, s.size());
    s.insert(0, precision, '0');
    std::string result_str = str + s + str1;

    // Allocate memory for const char*
    const char* result_cstr = new char[result_str.size() + 1];

    // Copy string to const char*
    std::strcpy(const_cast<char*>(result_cstr), result_str.c_str());

    // Create vector of const char*
    std::vector<const char*> result_vector {result_cstr};

    return result_vector;
}

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
	TTF_Init();

	window.create("Project Taiko", GAME_WIDTH, GAME_HEIGHT);

	donSfx = Mix_LoadWAV("res/sounds/taiko-normal-hitnormal.wav");
	kaSfx = Mix_LoadWAV("res/sounds/taiko-normal-hitclap.wav");
	missSfx = Mix_LoadWAV("res/sounds/taiko-normal-hitwhistle.wav");
	reAoharuMusic = Mix_LoadMUS("res/sounds/reaoharu-audio.mp3");

	donTexture = window.loadTexture("res/textures/don.png");
	kaTexture = window.loadTexture("res/textures/ka.png");

	//base.push_back(Entity(Vector2f(0,0), window.loadTexture("res/textures/background.png")));
	base.push_back(Entity(Vector2f(0, PLACEMENT), window.loadTexture("res/textures/taiko-bar-left.png")));
	base.push_back(Entity(Vector2f(190, PLACEMENT), window.loadTexture("res/textures/taiko-bar-middle.png")));
	base.push_back(Entity(Vector2f(390, PLACEMENT), window.loadTexture("res/textures/taiko-bar-right.png")));
	
    for (int i = 1; i <= 723; i++) {
        std::vector<const char*> v = getNumber("res/textures/background/Image_", i, ".jpg");
        bgfile.insert(bgfile.end(), v.begin(), v.end());
    }
  
   // for (auto i : bgfile) std::cout << i << std::endl;

    for (auto i: bgfile)
    {
    	backgrounds.push_back(Entity(Vector2f(0,0), window.loadTexture(i)));
    }

    // Free memory for const char*
    for (auto i : bgfile) delete[] i;

    shiroko.push_back(Entity(Vector2f(0, 18), window.loadTexture("res/textures/shiroko/0.png")));
	shiroko.push_back(Entity(Vector2f(0, 18), window.loadTexture("res/textures/shiroko/1.png")));
	shiroko.push_back(Entity(Vector2f(0, 18), window.loadTexture("res/textures/shiroko/2.png")));
	shiroko.push_back(Entity(Vector2f(0, 18), window.loadTexture("res/textures/shiroko/3.png")));

	innerTexture = window.loadTexture("res/textures/taiko-drum-inner.png");
	outerTexture = window.loadTexture("res/textures/taiko-drum-outer.png");
	hyperTexture = window.loadTexture("res/textures/taiko-glow.png");
	perfectTexture =  window.loadTexture("res/textures/taiko-hit-perfect.png");
	okTexture =  window.loadTexture("res/textures/taiko-hit-ok.png");
	missTexture =  window.loadTexture("res/textures/taiko-hit-miss.png");

	font64 = TTF_OpenFont("res/fonts/taikofont.ttf", 64);
	font64_outline = TTF_OpenFont("res/fonts/taikofont.ttf", 64);
	font32 = TTF_OpenFont("res/fonts/taikofont.ttf", 32);
	font32_outline = TTF_OpenFont("res/fonts/taikofont.ttf", 32);
	font24 = TTF_OpenFont("res/fonts/taikofont.ttf", 24);
	font16 = TTF_OpenFont("res/fonts/taikofont.ttf", 16);
	TTF_SetFontOutline(font32_outline, 1);
	TTF_SetFontOutline(font64_outline, 1);  
}

void pressNote(SDL_Texture* p_tex, Uint32 time)
{
	if (playnotes.size() <= 0) return;
	float distance = playnotes[0].distanceFromPoint(playnotes[0].getPos());
	//std:: cout << distance << std::endl; 
	if (playnotes[0].getTex() != p_tex) return;
	if (distance <= 100) 
	{
		if (distance <= 40)
		{
			effect.push_back(Effect(Vector2f(190, PLACEMENT), perfectTexture, time));
			point += PERFECT;
			combo++;
			scores[0]++;
		} else if (distance <= 70)
			{
				effect.push_back(Effect(Vector2f(190, PLACEMENT), okTexture, time));
				point += OKAY;
				scores[1]++;
				combo++;
			} else
				{
					Mix_PlayChannel(-1, missSfx, 0);
					effect.push_back(Effect(Vector2f(190, PLACEMENT), missTexture, time));
					scores[2]++;
					combo = 0;
				}
		playnotes.erase(playnotes.begin());
	}
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
	int currentBackGround = 0;
	int shirokoBackGround = 0;
	int finalNote = reaoharu.size();

	Uint32 currentTime;
	Uint32 backGroundTime;
	Uint32 shirokoTime;
	Uint32 lastBackGroundTime = 0;
	Uint32 lastShirokoTime = 0;
	Uint32 initNote = SDL_GetTicks();
	Uint32 nextNoteTime;
	Uint32 lastFrameTime = SDL_GetTicks();

	Mix_PlayMusic(reAoharuMusic, -1);

	SDL_Event event;

	while (gameRunning)
	{
		window.clear();

		currentTime = SDL_GetTicks();
		backGroundTime = currentTime - lastBackGroundTime;
		shirokoTime = currentTime - lastShirokoTime;
		nextNoteTime = timings[currentNote] - GAME_WIDTH + initNote;

		if (currentTime >= nextNoteTime)
		{			
			if (currentNote < finalNote) 
			{
				playnotes.push_back(reaoharu[currentNote]);
				currentNote++;
			}
		}

  		Uint32 deltaTime = currentTime - lastFrameTime; // calculate elapsed time
  		//std::cout << deltaTime << std::endl;
        lastFrameTime = currentTime; // update last frame time

		for (auto it = playnotes.begin(); it != playnotes.end(); ) 
		{
			it->setPos(it->moveNote(it->getPos(), deltaTime));
			if (it->getPos().x <= 100 && playnotes.size() > 0) {
				it = playnotes.erase(it);
				effect.push_back(Effect(Vector2f(190, PLACEMENT), missTexture, currentTime));
				Mix_PlayChannel(-1, missSfx, 0);
				scores[2]++;
				combo = 0;
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
							Mix_PlayChannel(-1, donSfx, 0);
							drum.push_back(Effect(Vector2f(0, PLACEMENT), outerTexture, currentTime));
							pressNote(kaTexture, currentTime);
							break;
						case SDLK_RIGHT:
							Mix_PlayChannel(-1, kaSfx, 0);
							drum.push_back(Effect(Vector2f(0, PLACEMENT), innerTexture, currentTime));
							pressNote(donTexture, currentTime);
							break;
				}
			}
		}

		/*int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		std::cout << "Mouse X: " << mouseX << ", Mouse Y: " << mouseY << std::endl;*/

		if (backGroundTime >= 200 && currentBackGround < 723) 
		{
			playbackground.push_back(backgrounds[currentBackGround]);
			lastBackGroundTime = currentTime;
			currentBackGround++;
			//std::cout << currentBackGround << std::endl;
		}

		window.render(playbackground[playbackground.size()-1]);

		for (Entity& p : base) window.render(p);
		window.render(Vector2f(1140, 20), getScore(point), font32, blue);
		window.render(Vector2f(1175, 60), getScore(scores[0], scores[1], scores[2]), font24, white);
		window.render(Vector2f(20, 600), getScore("Perfect: ", scores[0]), font24, white);
		window.render(Vector2f(20, 630), getScore("Okay: ", scores[1]), font24, white);
		window.render(Vector2f(20, 660), getScore("Miss: ", scores[2]), font24, white);
		
		window.renderCenter(Vector2f(0, -GAME_HEIGHT/3), getScore("", combo), font64, blue);
		window.renderCenter(Vector2f(0, -GAME_HEIGHT/3+40), "Combo", font24, white);
		
		window.renderCenter(Vector2f(0, -GAME_HEIGHT/3), getScore("", combo), font64_outline, black);
		for (Effect& p : effect) window.render(p);
		for (Note& p : playnotes) window.render(p);
		window.render(base[0]);

		//window.render(shiroko[0]);
		if (shirokoTime >= 60) 
		{
			lastShirokoTime = currentTime;
			shirokoBackGround++;
			if (shirokoBackGround == 3) shirokoBackGround = 0;
		}
		window.render(shiroko[shirokoBackGround]);
		
		for (Effect& p: drum) window.render(p);

		for (Effect& p : effect) {
			if (currentTime - p.getTime() > 100 && effect.size() > 0) 
			{
				effect.erase(effect.begin());
			}
		}
		for (Effect& p : drum) {
			if (currentTime - p.getTime() > 100 && drum.size() > 0) 
			{
				drum.erase(drum.begin());
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
	TTF_CloseFont(font32);
	TTF_CloseFont(font32_outline);
	TTF_CloseFont(font24);
	TTF_CloseFont(font16);
	Mix_Quit();
	SDL_Quit();

	return 0;
}