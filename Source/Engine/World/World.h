
#include <iostream>
#include <SDL.h>

class World
{
public:
	bool Start() {
		if (SDL_Init(SDL_INIT_VIDEO) != 0) {
			std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
			return false;
		}
		SDL_Quit();
		return true;
	}
};
