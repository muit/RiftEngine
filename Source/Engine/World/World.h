
#include <iostream>
#include <SDL.h>

#include "EngineTypes.h"
#include "Reflection/Reflection.h"


class World {
	CLASS(World)

	PROP(int16, name, "Save")
	int16 name;

public:
	bool Start() {
		if (SDL_Init(SDL_INIT_VIDEO) != 0) {
			std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
			return false;
		}
		std::cout << "SDL Okay!" << std::endl;

		auto Property = World::StaticClass()->FindPropertyHandle<int16>(*this, "name");
		Property.SetValue(160);

		std::cout << name << std::endl;

		std::cout << (Property.HasTag("Save")? "true" : "false") << std::endl;

		SDL_Quit();
		return true;
	}
};
