// Copyright 2015-2019 Piperift - All rights reserved

#include "Engine.h"

#include <SDL_image.h>

Ptr<Engine> Engine::globalEngine {};


bool Engine::Start()
{
	Frame frame = {};

	{
		ZoneScopedN("Start-Up");

		// Setup SDL
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
			return false;

		u32 imgFlags = IMG_INIT_PNG;
		if (!IMG_Init(imgFlags) & imgFlags)
			return false;

		renderer = Create<Renderer>();
		if (!renderer->Initialize())
			return false;



		world = Create<World>(GetSelf());
		world->Start(frame);

		ui = Create<UIManager>(GetSelf());
		ui->Prepare();
	}

	frameTime = {};
	bool bFinish = false;
	while (!bFinish)
	{
		Loop(frame, bFinish);

		// Reset frame data
		frame = {};
	}

	return true;
}

void Engine::Loop(Frame& frame, bool& bFinish)
{
	ZoneScopedNC("Game", 0x459bd1);

	frameTime.Tick();

	{
		ZoneScopedNC("Input", 0x459bd1);
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

	// Rendering
	renderer->Render(frame);

	renderer->Sleep();
	FrameMark;
}
