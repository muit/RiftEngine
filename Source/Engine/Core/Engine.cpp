// Copyright 2015-2019 Piperift - All rights reserved

#include "Engine.h"

Ptr<Engine> Engine::globalEngine {};


bool Engine::Start()
{
	{
		ZoneScopedN("Start-Up");

		// Setup SDL
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
			return false;

		renderer = Create<Renderer>();
		if (!renderer->Initialize())
			return false;

		world = Create<World>(GetSelf());
		world->Start();

		ui = Create<UIManager>(GetSelf());
		ui->Prepare();
	}

	frameTime = {};
	bool bFinish = false;
	while (!bFinish)
	{
		Loop(bFinish);
	}

	return true;
}

void Engine::Loop(bool& bFinish)
{
	Frame frame = {};
	{
		ZoneScopedN("Game");

		frameTime.Tick();

		{
			ZoneScopedN("Input");
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
		}

		renderer->PreTick();

		world->Tick(frame, frameTime.deltaTime);
		ui->Tick(frameTime.deltaTime);
	}

	// Rendering
	renderer->Render(frame);

	renderer->Sleep();
	FrameMark;
}
