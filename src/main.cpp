#include "Game.hpp"

Game* game = NULL;

int main(int argc, char* argv[])
{
	game = new Game();
	game->init();
	while (game->running())
	{
		game->handleEvents();
		game->update();
		game->render();
		/*int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		std::cout << "Mouse X: " << mouseX << ", Mouse Y: " << mouseY << std::endl;*/
	}
	game->clean();
	return 0;
}