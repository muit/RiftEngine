// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "SDL.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

class Viewport {

public:

	Viewport() {}

	bool Start() {
		if(SDL_Init(SDL_INIT_VIDEO) < 0)
			return false;

		SDL_Window* window = SDL_CreateWindow(
			"FecoEngine",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN
		);

		if (window == NULL)
			return false;

		SDL_Surface* screenSurface = SDL_GetWindowSurface(window);
		SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
		SDL_UpdateWindowSurface(window);
		SDL_Delay(2000);
		SDL_DestroyWindow(window);
		SDL_Quit();

		return true;
	}
};
