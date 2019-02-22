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

		input = Create<InputManager>();


		world = Create<World>(GetSelf());
		world->Start(frame);

		ui = Create<UIManager>(GetSelf());
		ui->Prepare();
	}

	frameTime = {};
	frameTime.SetFPSCap(60);

	bool bFinish = false;
	while (!bFinish)
	{
		Loop(frame, bFinish);
	}

	return true;
}

void Engine::Loop(Frame& frame, bool& bFinish)
{
	frameTime.Tick();

	bFinish = input->Tick(frameTime.GetDeltaTime(), ui, renderer);

	renderer->PreTick();

	{
		ZoneScopedNC("Game", 0x459bd1);
		world->Tick(frame, frameTime.GetDeltaTime());
		ui->Tick(frameTime.GetDeltaTime());
	}

	// Rendering
	renderer->Render(frame);

	// Reset frame data
	frame = {};

	frameTime.PostTick();
	FrameMark;
}
