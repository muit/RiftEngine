
#include <iostream>
#include <SDL.h>

#include "Reflection/Reflection.h"

class World {
	CLASS(World)

	PROP(std::string, name)
	std::string name;

public:
	bool Start() {
		if (SDL_Init(SDL_INIT_VIDEO) != 0) {
			std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
			return false;
		}
		std::cout << "SDL Okay!";

		auto Property = World::StaticClass()->FindPropertyHandle<std::string>(*this, "name");
		Property.SetValue("Meeh");

		std::cout << name;

		SDL_Quit();
		return true;
	}
};
