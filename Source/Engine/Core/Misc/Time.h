// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "EngineTypes.h"
#include "SDL_timer.h"
#include "DateTime.h"


struct FrameTime
{
protected:

	DateTime previousTime;
	DateTime currentTime;

	float realDeltaTime;
	float deltaTime;

	float timeDilation;

	// In essence 1/FPS_CAP
	float minFrameTime;

public:

	FrameTime()
		: previousTime{}
		, currentTime{ DateTime::Now() }
		, realDeltaTime(0.f)
		, deltaTime(0.f)
		, timeDilation(1.f)
	{}

	void Tick()
	{
		previousTime = currentTime;
		currentTime = DateTime::Now();

		// Avoid too big delta times
		realDeltaTime = Math::Min(0.15f, (currentTime - previousTime).GetTotalSeconds());


		// Apply time dilation
		deltaTime = realDeltaTime * timeDilation;
	}

	void PostTick()
	{
		float extraTimeForFPSCAP = minFrameTime - (DateTime::Now() - currentTime).GetTotalSeconds();
		if (extraTimeForFPSCAP > 0.0f)
		{
			ZoneScopedNC("Sleep", 0xD15545);
			// Cap FPS
			SDL_Delay(i32(extraTimeForFPSCAP * 1000.f));
		}
	}

	void SetFPSCap(u32 maxFPS) {
		minFrameTime = 1.f / maxFPS;
	}

	void SetTimeDilation(float newTimeDilation) {
		timeDilation = newTimeDilation;
	}

	FORCEINLINE float GetDeltaTime() const {
		return deltaTime;
	}
};
