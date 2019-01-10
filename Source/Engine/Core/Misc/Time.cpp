// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "EngineTypes.h"
#include "SDL_timer.h"


struct FrameTime
{
	FrameTime() : lastTick(SDL_GetPerformanceCounter()), deltaTime(0.f) {}

	u64 currentTick;
	u64 lastTick;
	float deltaTime;

	void Tick()
	{
		lastTick = currentTick;
		currentTick = SDL_GetPerformanceCounter();

		deltaTime = (currentTick - lastTick) / (float)SDL_GetPerformanceFrequency();
	}
};
