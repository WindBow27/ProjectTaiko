#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cmath>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Math.hpp"
#include "Note.hpp"
#include "Effect.hpp"
#include "Utils.hpp"
#include "Selection.hpp"

Selection::Selection(RenderWindow p_window) : window(p_window)
{
	screenTexture = window.loadTexture("res/textures/selection/selection.png");
	buttonTexture = window.loadTexture("res/textures/button.png");
	buttonEasyTexture = window.loadTexture("res/textures/selection/easy.png");
	buttonNormalTexture = window.loadTexture("res/textures/selection/normal.png");
	buttonHardTexture = window.loadTexture("res/textures/selection/hard.png");
	preciseTexture = window.loadTexture("res/textures/selection/bindings/Precise.png");
	suddendeathTexture = window.loadTexture("res/textures/selection/bindings/Sudden-Death.png");
	allperfectTexture = window.loadTexture("res/textures/selection/bindings/All-Perfect.png");
	bindinglightTexture = window.loadTexture("res/textures/selection/bindings/Binding-Light.png");
	precise1Texture = window.loadTexture("res/textures/selection/bindings/Precise-1.png");
	suddendeath1Texture = window.loadTexture("res/textures/selection/bindings/Sudden-Death-1.png");
	allperfect1Texture = window.loadTexture("res/textures/selection/bindings/All-Perfect-1.png");
	bindinglight1Texture = window.loadTexture("res/textures/selection/bindings/Binding-Light-1.png");
	desc1 = window.loadTexture("res/textures/selection/bindings/desc1.png");
	desc2 = window.loadTexture("res/textures/selection/bindings/desc2.png");
	desc3 = window.loadTexture("res/textures/selection/bindings/desc3.png");
	desc4 = window.loadTexture("res/textures/selection/bindings/desc4.png");
	rankS = window.loadTexture("res/textures/selection/ranks/ranking-S.png");
	rankA = window.loadTexture("res/textures/selection/ranks/ranking-A.png");
	rankB = window.loadTexture("res/textures/selection/ranks/ranking-B.png");
	rankC = window.loadTexture("res/textures/selection/ranks/ranking-C.png");
	rankD = window.loadTexture("res/textures/selection/ranks/ranking-D.png");

	font32 = TTF_OpenFont("res/fonts/taikofont.ttf", 32);
	font40 = TTF_OpenFont("res/fonts/taikofont.ttf", 40);
	font32_outline = TTF_OpenFont("res/fonts/taikofont.ttf", 32);
	TTF_SetFontOutline(font32_outline, 1);
	button[EASY] = new Button(window, "Easy", font32, black, buttonEasyTexture, Vector2f(487, 100));
	button[NORMAL] = new Button(window, "Normal", font32, black, buttonNormalTexture, Vector2f(487, 220));
	button[HARD] = new Button(window, "Hard", font32, black, buttonHardTexture, Vector2f(487,340));
	button[BACK] = new Button(window, "Back", font32, black, buttonTexture, Vector2f(50, 600));
	button[EASY]->setSize(Vector2f(306, 96));
	button[NORMAL]->setSize(Vector2f(306, 96));
	button[HARD]->setSize(Vector2f(306, 96));

	binding[PRECISE] = new Button(window, " ", font40, black, preciseTexture, Vector2f(134, 150));
	binding[SUDDENDEATH] = new Button(window, " ", font40, black, suddendeathTexture, Vector2f(120, 300));
	binding[ALLPERFECT] = new Button(window, " ", font40, black, allperfectTexture, Vector2f(1048, 150));
	binding[BINDINGLIGHT] = new Button(window, " ", font40, black, bindinglightTexture, Vector2f(1062, 300));
	for (auto i: binding) i->setSize(Vector2f(100,125));

	binding[PRECISE]->setOnClickTexture(precise1Texture);
	binding[SUDDENDEATH]->setOnClickTexture(suddendeath1Texture);
	binding[ALLPERFECT]->setOnClickTexture(allperfect1Texture);
	binding[BINDINGLIGHT]->setOnClickTexture(bindinglight1Texture);

	binding[PRECISE]->setHoverTexture(desc1);
	binding[SUDDENDEATH]->setHoverTexture(desc2);
	binding[ALLPERFECT]->setHoverTexture(desc3);
	binding[BINDINGLIGHT]->setHoverTexture(desc4);
}

void Selection::init()
{
	highscore.clear();
	std::fstream scoreFile("score.txt");
	int rank;
	while (scoreFile >> rank)
	{
		highscore.push_back(rank);
	}
	scoreFile.close();

	if (highscore.size() < 3) highscore = {999, 999, 999}; //handling first playthrough
	std::ofstream outputScoreFile("score.txt");
	for (auto i: highscore) outputScoreFile << i << std::endl;
	outputScoreFile.close();
}

void Selection::update()
{
	for (auto i:button) i->update();
	for (auto i:binding) i->update();
	for (int i = 0; i < 4; i++)
	{
		mod[i] = binding[i]->getClicked();
		//std::cout << mod[i] << " ";
	}
}

void Selection::setRank(int i, SDL_Texture* rankTexture) 
{
	switch (i) 
	{
	case 0:
		window.render(Vector2f(710, 127.5), rankTexture);
		break;
	case 1:
		window.render(Vector2f(710, 247.5), rankTexture);
		break;
	case 2:
		window.render(Vector2f(710, 367.5), rankTexture);
		break;
	}
}

void Selection::render()
{
	window.clear();
	window.render(Vector2f(0, 0), screenTexture);
	for (auto i:button) i->render();
	for (int i = 0; i < 3; i++) {
		switch (highscore[i])
		{
		case 0:
			setRank(i, rankS);
			break;
		case 1:
			setRank(i, rankA);
			break;
		case 2:
			setRank(i, rankB);
			break;
		case 3:
			setRank(i, rankC);
			break;
		case 4:
			setRank(i, rankD);
			break;
		default:
			break;
		}	
	}
	for (auto i:binding) i->render();
	window.display();
}

Button* Selection::getButton(int num)
{
	return button[num];
}

std::vector<bool> Selection::getMod()
{
	return mod;
}