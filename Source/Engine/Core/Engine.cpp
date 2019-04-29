// Copyright 2015-2019 Piperift - All rights reserved

#include "Engine.h"

#include <SDL_image.h>
#include "Tools/Profiler.h"


Ptr<Engine> Engine::globalEngine {};

bool Engine::Start()
{
	{
		Log::Message("Start-Up");
		ScopedGameZone("Start-Up");

		// Setup SDL
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
		{
			Log::Error("Could not initialize SDL.");
			return false;
		}

		u32 imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			Log::Error("Could not initialize SDL Img library.");
			return false;
		}

		renderer = Create<Renderer>(Self());
		if (!renderer->Initialize())
			return false;

		input = Create<InputManager>(Self());
		assetManager = Create<AssetManager>(Self());

		world = Create<World>(Self());
		world->Initialize();

		ui = Create<UIManager>(Self());
		ui->Prepare();
	}

	frameTime = {};
	frameTime.SetFPSCap(60);

	Log::Message("Start Loop");
	bool bFinish = false;
	while (!bFinish)
	{
		Loop(bFinish);
	}

	Log::Message("Closing");
	return true;
}

void Engine::Loop(bool& bFinish)
{
	frameTime.Tick();

	bFinish = input->Tick(frameTime.GetDeltaTime(), ui, renderer);

	renderer->PreTick();

	{
		ScopedGameZone("Game");
		world->Tick(frameTime.GetDeltaTime());
		ui->Tick(frameTime.GetDeltaTime());
	}

	// Rendering
	renderer->Render();

	frameTime.PostTick();
	FrameMark;
}
