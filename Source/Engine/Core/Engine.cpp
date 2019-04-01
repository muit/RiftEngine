// Copyright 2015-2019 Piperift - All rights reserved

#include "Engine.h"

#include <SDL_image.h>

Ptr<Engine> Engine::globalEngine {};


bool Engine::Start()
{
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


		world = Create<World>(Self());
		world->Start();

		ui = Create<UIManager>(Self());
		ui->Prepare();
	}

	frameTime = {};
	frameTime.SetFPSCap(60);

	bool bFinish = false;
	while (!bFinish)
	{
		Loop(bFinish);
	}

	return true;
}

void Engine::Loop(bool& bFinish)
{
	frameTime.Tick();

	bFinish = input->Tick(frameTime.GetDeltaTime(), ui, renderer);

	renderer->PreTick();

	{
		ZoneScopedNC("Game", 0x459bd1);
		world->Tick(frameTime.GetDeltaTime());
		ui->Tick(frameTime.GetDeltaTime());
	}

	// Rendering
	renderer->Render();

	frameTime.PostTick();
	FrameMark;
}
