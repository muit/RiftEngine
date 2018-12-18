// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreEngine.h"
#include "Renderer.h"
#include <SDL.h>
#include <tracy/Tracy.hpp>

#include "Object.h"
#include "Util/Time.h"
#include "World/World.h"
#include "UI/UIManager.h"


class Engine : public Object {
	CLASS(Engine, Object)

	FrameTime frameTime;
	GlobalPtr<World> world;
	GlobalPtr<UIManager> ui;
	GlobalPtr<Renderer> renderer;

	PROP(uint8, time)
	uint8 time;

public:

	Engine() { time = 3; }


	/** Begin Object interface */
protected:
	virtual void BeforeDestroy() override { Shutdown(); }
public:
	virtual Ptr<World> GetWorld() const override { return world; }
	/** End Object interface */


	bool Start() {
		// Setup SDL
		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
			return false;

		renderer = Create<Renderer>();
		if(renderer->GetState() == ERendererState::Failed)
			return false;

		GetSelf();
		world = Create<World>(GetSelf());
		world->Start();

		ui = Create<UIManager>(GetSelf());
		ui->Prepare();

		frameTime = {};
		bool bFinish = false;
		while (!bFinish)
		{
			TracyMessageL("Tick");
			ZoneScopedN("Tick");

			frameTime.Tick();

			// Process window and input events
			SDL_PumpEvents();
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				ui->OnSDLEvent(&event);
				if (event.type == SDL_QUIT)
					bFinish = true;
				else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == renderer->GetWindowId())
					bFinish = true;
			}

			renderer->PreTick();

			world->Tick(frameTime.deltaTime);
			ui->Tick(frameTime.deltaTime);

			// Rendering
			renderer->Render();
		}

		return true;
	}

	void Shutdown() {
		SDL_Quit();
	}


	static void StartEngine()
	{
		GEngine->Start();
	}

	static const GlobalPtr<Engine> GEngine;
};

#define GEngine Engine::GEngine
