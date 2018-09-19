// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <SDL.h>

#include "Renderer.h"


class Engine : public Object {

	GlobalPtr<Renderer> renderer;

public:

	Engine() {}

	bool Start() {
		// Setup SDL
		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
			return false;

		renderer = Create<Renderer>();
		if(renderer->GetState() == ERendererState::Failed)
			return false;

		bool bFinish = false;
		while (!bFinish)
		{
			// Process window and input events
			SDL_PumpEvents();
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					bFinish = true;
				else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == renderer->GetWindowId())
					bFinish = true;
			}

			// Start the Dear ImGui frame
			renderer->PreTick();

			// Tick
			renderer->TickUI(0.0f);

			// Rendering
			renderer->Render();
		}
		return true;
	}

	~Engine() {
		SDL_Quit();
	}
};
