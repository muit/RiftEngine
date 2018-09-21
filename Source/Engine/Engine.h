// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <SDL.h>

#include "Renderer.h"
#include "UI/UIManager.h"


class Engine : public Object {
	CLASS(Engine, Object)

	GlobalPtr<UIManager> ui;
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

		ui = Create<UIManager>(ThisPtr());
		ui->Prepare();

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
			ui->Tick(0.f);

			// Rendering
			renderer->Render();
		}

		return true;
	}

	void Shutdown() {
		SDL_Quit();
	}

	//virtual void BeforeDestroy() override { Shutdown(); }


	static void StartEngine()
	{
		GEngine->Start();
	}

	static const GlobalPtr<Engine> GEngine;
};

const GlobalPtr<Engine> Engine::GEngine{ Create<Engine>() };
#define GEngine Engine::GEngine;
